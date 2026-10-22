/*
 * A399197  Number of nonnegative integer solutions to the equation x^2 +
 *          4*y^2 = n.
 * https://oeis.org/A399197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of nonnegative integer solutions (x, y) to the
 *            equation x^2 + 4*y^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation loops over all possible y with 
   4*y^2 ≤ n and counts the pairs for which the remaining
   value is a perfect square. This is O(√n), fast enough
   for n up to at least 10⁴. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t y = 0; 4 * y * y <= n; ++y) {
        idx_t rem = n - 4 * y * y;
        seq_t root = floor_sqrt((seq_t)rem);
        if (root * root == rem) {
            count += 1;
        }
    }
    return count;
}
