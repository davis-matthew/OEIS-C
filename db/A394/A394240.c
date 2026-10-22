/*
 * A394240  Number of regions formed by extending the faces of a regular
 *          n-gonal pyramid.
 * https://oeis.org/A394240
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2-n+6)/2 for n odd, a(n) = (3*n^2-2*n+6)/2 for n
 *            even
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result;
    if ((n & 1) != 0) /* n is odd */
    {
        result = (3 * nn * nn - nn + 6) / 2;
    }
    else /* n is even */
    {
        result = (3 * nn * nn - 2 * nn + 6) / 2;
    }
    return result;
}
