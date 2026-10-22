/*
 * A326289  a(0) = 0, a(n) = 2^binomial(n,2) - 2^(n - 1).
 * https://oeis.org/A326289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^C(n,2) - 2^(n-1) (C(n,2) = n*(n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* a(0) = 0 by definition */

    /* exponent for the first power of 2 */
    idx_t exp_first = n * (n - 1) / 2;

    seq_t result = pow2(exp_first);  /* 2^C(n,2)  */
    result -= pow2(n - 1);           /* subtract 2^(n-1) */

    return result;
}
