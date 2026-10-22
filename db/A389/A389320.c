/*
 * A389320  Upper (3/4)-midsequence of square numbers and negative triangular
 *          numbers; see Comments.
 * https://oeis.org/A389320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((3/8)*(n^2 - n))
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* convert index to seq_t */
    seq_t num = n_t * n_t;         /* n^2 */
    num -= n_t;                    /* n^2 - n = n*(n-1) */
    num *= (seq_t)3;               /* 3 * n*(n-1) */
    /* Ceil division by 8: (num + 8 - 1) / 8 */
    return (num + (seq_t)7) / (seq_t)8;
}
