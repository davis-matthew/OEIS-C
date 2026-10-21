/*
 * A269156  Smallest sum of length of edges and diagonals of an integer
 *          polygon with n vertices.
 * https://oeis.org/A269156
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*n*(n-1) for n >= 5 a(3) = 3, a(4) = 19 (return 0 for n
 *            < 3, though such calls are outside the OEIS offset)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)19;
    if (n < 3)  return (seq_t)0; /* undefined region, but harmless */

    seq_t nn = (seq_t)n;
    return nn * (nn - (seq_t)1) * (seq_t)3;
}
