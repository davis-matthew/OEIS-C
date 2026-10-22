/*
 * A336911  Image of n under the x^3+1 map, which is a variation of the 3x+1
 *          (Collatz) map.
 * https://oeis.org/A336911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor_sqrt(n) if n is even, n^3+1 if n is odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* n is even */
        return floor_sqrt((seq_t)n);

    /* n is odd */
    seq_t m = (seq_t)n;
    return m * m * m + 1;
}
