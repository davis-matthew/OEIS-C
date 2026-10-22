/*
 * A394684  Number of ordered pairs of integers (x,y) with 0 <= x <= y and
 *          x^2+y^2 < n^2.
 * https://oeis.org/A394684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # { (x,y) | 0 <= x <= y, x^2 + y^2 < n^2 }
 * Range:     u8 n <= 8, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;

    for (idx_t x = 0; x < n; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        seq_t rem = n2 - x2;          /* n^2 - x^2, positive while x < n */

        if (rem == 0) continue;       /* no y satisfies y^2 < 0 */
        seq_t ymax = floor_sqrt(rem - 1);  /* floor(sqrt(rem−1)) */

        seq_t sx = (seq_t)x;
        if (ymax < sx) continue;      /* y ≥ x condition fails */

        total += (ymax - sx + 1);     /* count y = x … ymax */
    }

    return total;
}
