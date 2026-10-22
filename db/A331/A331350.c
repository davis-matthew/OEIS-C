/*
 * A331350  Number of oriented colorings of the edges (or triangular faces) of
 *          a regular 4-dimensional simplex with n available colors.
 * https://oeis.org/A331350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*n^2 + 20*n^4 + 15*n^6 + n^10) / 60
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast index to seq_t */
    seq_t ns = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n6 = n4 * n2;          /* n^6 */
    seq_t n10 = n6 * n4;         /* n^10 */

    /* numerator of the formula */
    seq_t num = n10
              + n6 * 15
              + n4 * 20
              + n2 * 24;

    /* divide by 60 - division is exact according to the formula */
    return num / 60;
}
