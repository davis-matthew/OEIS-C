/*
 * A393710  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are positive integers satisfying x < y < z.
 * https://oeis.org/A393710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples of positive integers (x,y,z) with x < y
 *            < z such that x^2 + y*z = n
 * Range:     u8 n <= 64, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* x must satisfy x^2 < n, otherwise no solution */
    for (idx_t x = 1; ; ++x) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        if (x_sq >= (seq_t)n) break;
        seq_t m = (seq_t)n - x_sq;          /* m = y*z */
        if (m <= 0) continue;               /* safety for n very small */

        /* y ranges from x+1 up to floor(sqrt(m)) */
        idx_t y_max = (idx_t)floor_sqrt(m);
        for (idx_t y = x + 1; y <= y_max; ++y) {
            if ((m % (seq_t)y) == 0) {      /* y divides m */
                seq_t z = m / (seq_t)y;     /* z = m / y */
                if (z > (seq_t)y)           /* enforce y < z */
                    ++count;
            }
        }
    }
    return count;
}
