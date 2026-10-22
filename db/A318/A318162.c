/*
 * A318162  Number of compositions of 2n-1 into exactly 2n-1 nonnegative parts
 *          with largest part n.
 * https://oeis.org/A318162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 2*(2*n-1)*binomial(3*n-4, n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t n3_minus4 = 3 * n - 4;    /* 3*n-4 */
    idx_t nk_minus2 = n - 2;       /* n-2 */

    seq_t bin = binomial(n3_minus4, nk_minus2);   /* binomial(3*n-4, n-2) */
    seq_t two_n = (seq_t)2 * (seq_t)n;             /* 2*n */
    seq_t two_n_minus1 = two_n - (seq_t)1;        /* 2*n-1 */
    seq_t mult = (seq_t)2 * two_n_minus1;         /* 2*(2*n-1) */

    return bin * mult;
}
