/*
 * A296770  Row sums of A050158.
 * https://oeis.org/A296770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 2) * binomial(2*n + 1, n) - 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute the binomial coefficient C(2*n + 1, n) */
    seq_t bin = binomial(2*n + 1, n);

    /* compute 4^n */
    seq_t pow4 = pow_uint(4, n);

    /* compute (n + 2) * bin */
    seq_t mult = ((seq_t)(n + 2)) * bin;

    /* a(n) = (n + 2) * C(2*n + 1, n) - 4^n */
    return mult - pow4;
}
