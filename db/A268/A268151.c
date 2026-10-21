/*
 * A268151  A double binomial sum involving absolute values.
 * https://oeis.org/A268151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(4*n-1)*n*(6*n-1); a(0) = 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute 2^(4*n-1) */
    seq_t two_pow = pow2(4 * n - 1);

    /* Compute n * (6*n - 1) safely in seq_t */
    seq_t inner = (seq_t)n * (((seq_t)6 * (seq_t)n) - (seq_t)1);

    /* Final product */
    return two_pow * inner;
}
