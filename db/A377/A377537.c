/*
 * A377537  a(n) is the number of positive integers that have n prime factors
 *          and these are all <= n.
 * https://oeis.org/A377537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(π(n)+n-1, n) where π(n) is the prime counting
 *            function
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 36, u64 n <= 72, u128 n <= 156,
 *            big any n
 * Helpers:   build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/compute_pi.h"
#include "../utils/build_primes_upto.h"

static inline idx_t legendre(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

static inline seq_t mul_seq_t(seq_t x, seq_t y)
{
    /* Use 128‑bit intermediate for 64‑bit builds, otherwise rely on the
       bigint operator* defined for the seq_t type. */
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((__int128)x * y);
    } else {
        return x * y;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* offset 1, a(1)=0 */

    /* ------------------------------------------------------------
       Compute π(n) and a suitable set of primes up to total = π(n)+n-1
       ------------------------------------------------------------ */
    idx_t pi;                       /* number of primes ≤ n   */
    {
        size_t cnt;
        uint64_t *primes;
        build_primes_upto(n, &primes, &cnt);
        pi = (idx_t)cnt;
        free(primes);
    }

    idx_t total = pi + n - 1;        /* argument of the binomial */

    /* Build all primes needed for the factorisation of C(total, n)   */
    size_t pcnt;
    uint64_t *primes;
    build_primes_upto(total, &primes, &pcnt);

    /* ------------------------------------------------------------
       Compute C(total, n) by prime factorisation to avoid overflow
       ------------------------------------------------------------ */
    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];

        /* exponent of p in C(total, n)   */
        idx_t e = legendre(total, p) - legendre(n, p) - legendre(total - n, p);
        if (e == 0) continue;

        /* compute p^e */
        seq_t power = 1;
        for (idx_t k = 0; k < e; ++k) {
            power = mul_seq_t(power, (seq_t)p);
        }

        /* multiply into the result */
        result = mul_seq_t(result, power);
    }
    free(primes);
    return result;
}
