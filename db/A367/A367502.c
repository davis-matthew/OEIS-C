/*
 * A367502  Sum of the final digits of the prime power divisors (p^k, k>=0) of
 *          n.
 * https://oeis.org/A367502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 + sum over all primes p dividing n of sum_{k=1..e} (p^k
 *            mod 10) where e is the exponent of p in n. p^0 contributes 1
 *            once
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 1)
        return (seq_t)1;

    /* Factorize n */
    uint64_t nn = (uint64_t)n;
    /* maximal number of distinct prime factors for 64‑bit n is 64 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    seq_t sum = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = ex[i];
        uint64_t modp = p % 10;      /* p modulo 10 once */
        uint64_t modpow = 1;          /* current power modulo 10 */

        for (uint64_t k = 1; k <= e; ++k) {
            modpow = (modpow * modp) % 10;   /* p^k mod 10 */
            sum += (seq_t)modpow;
        }
    }

    /* add the contribution of 1 (p^0) once */
    sum += (seq_t)1;

    return sum;
}
