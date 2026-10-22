/*
 * A348734  Numerator of Product((p+1)^e / ((p^e)+1)), when n = Product(p^e),
 *          with p primes, and e their exponents.
 * https://oeis.org/A348734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A003959(n)/gcd(A003959(n), A034448(n))
 * Range:     u8 n <= 47, u16 n <= 4095, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Return the numerator of the reduced product
   ∏ ( (p+1)^e / (p^e+1) ) over prime powers of n. */
seq_t a(idx_t n)
{
    /* n is at least 1; for n == 1 the empty product is 1. */
    uint64_t tmp = (uint64_t)n;

    /* arrays for prime factorisation; 64 is safely above the max
       number of distinct prime factors of a 64‑bit integer (≈20). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    factor_u64(tmp, primes, exps, &cnt);

    seq_t N = (seq_t)1;  /* numerator product */
    seq_t D = (seq_t)1;  /* denominator product */

    for (size_t k = 0; k < cnt; ++k) {
        uint64_t p = primes[k];
        uint64_t e = exps[k];

        /* (p+1)^e */
        seq_t num_part = (seq_t)1;
        for (uint64_t i = 0; i < e; ++i)
            num_part *= (seq_t)(p + 1);
        N *= num_part;

        /* p^e + 1 */
        seq_t den_part = (seq_t)1;
        for (uint64_t i = 0; i < e; ++i)
            den_part *= (seq_t)p;
        den_part += (seq_t)1;
        D *= den_part;
    }

    seq_t g = gcd(N, D);
    return N / g;
}
