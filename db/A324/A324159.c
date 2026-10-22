/*
 * A324159  Expansion of Sum_{k>=1} k * x^k / (1 - k * x^k)^k.
 * https://oeis.org/A324159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} (n/d)^d * binomial(n/d + d - 2, d - 1)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 47, u64 n <= 104, u128 n <= 221,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper functions provided by the build environment */
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/binomial.h"  /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/_floor_isqrt_u64.h" /* idx_t _floor_isqrt_u64(idx_t n); */

/* Compute the nth term of OEIS A324159 */
seq_t a(idx_t n)
{
    if (n == 0)        /* the sequence is defined for n ≥ 1, return 0 for safety */
        return 0;

    seq_t sum = 0;

    /* iterate over all divisors d of n */
    idx_t sqrt_n = _floor_isqrt_u64(n);
    for (idx_t d = 1; d <= sqrt_n; ++d)
    {
        if (n % d != 0)
            continue;

        /* first divisor */
        idx_t m = n / d;                                     /* m = n/d   */
        seq_t term = pow_uint((uint64_t)m, d);               /* (n/d)^d   */
        term = term * binomial(m + d - 2, d - 1);            /* * binomial  */
        sum = sum + term;                                    /* accumulate  */

        /* complementary divisor, if distinct */
        idx_t d2 = n / d;                                    /* d2 = n/d   */
        if (d2 != d)
        {
            idx_t m2 = n / d2;                               /* m2 = n/d2 = d */
            seq_t term2 = pow_uint((uint64_t)m2, d2);        /* (n/d2)^d2   */
            term2 = term2 * binomial(m2 + d2 - 2, d2 - 1);  /* * binomial  */
            sum = sum + term2;
        }
    }

    return sum;
}
