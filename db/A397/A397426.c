/*
 * A397426  Number of lines through 4 or more points of an n X n grid of
 *          points.
 * https://oeis.org/A397426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct lines in an n×n grid of points that
 *            pass through at least four of the grid points
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_int.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    seq_t res = (seq_t)n * 2;            /* horizontal + vertical */

    idx_t S = n - 1;                     /* last index 0..S */
    idx_t max_step = S / 3;              /* because 3*dx ≤ S and 3*dy ≤ S */

    for (idx_t dx = 1; dx <= max_step; ++dx) {
        for (idx_t dy = 1; dy <= max_step; ++dy) {
            if (gcd_int((int)dx, (int)dy) != 1) continue;

            idx_t lX = S - 3 * dx;
            idx_t lY = S - 3 * dy;

            /* lX & lY are always ≥ 0 by construction of max_step */
            seq_t total = (seq_t)(lX + 1) * (seq_t)(lY + 1);

            /* subtract the points that are too far from the left/bottom
               edges so that the line would not contain 4 points          */
            seq_t aX = (lX >= dx - 1) ? (seq_t)(lX - dx + 1) : 0;
            seq_t aY = (lY >= dy - 1) ? (seq_t)(lY - dy + 1) : 0;
            seq_t sub = aX * aY;

            seq_t lines_pos = total - sub;   /* count for +dy/dx slope */
            res += lines_pos << 1;           /* + and - slope */
        }
    }

    return res;
}
