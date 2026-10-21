/*
 * A278128  a(n) = 288*2^n - 156.
 * https://oeis.org/A278128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 288*2^n - 156
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using the provided pow2(n) utility */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t two_pow = pow2(n);

    /* Multiply by 288 */
    seq_t val = (seq_t)288 * two_pow;

    /* Subtract 156 */
    val = val - (seq_t)156;

    return val;
}
