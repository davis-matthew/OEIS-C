/*
 * A305159  a(n) = 102*2^n - 78.
 * https://oeis.org/A305159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*2^n - 78
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);          /* compute 2^n */
    p = p * (seq_t)102;         /* multiply by 102 */
    p = p - (seq_t)78;          /* subtract 78 */
    return p;
}
