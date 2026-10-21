/*
 * A242825  Minimum positive value of n^3 - x^3 - y^3 for positive integers
 *          x,y.
 * https://oeis.org/A242825
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum positive value of n^3 - x^3 - y^3 for all
 *            positive integers x, y (x,y > 0). Computed by brute‑force search
 *            over x and the cube‑root of the remainder
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 745, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* undefined for n < 2, not used */

    seq_t N   = (seq_t)n;             /* cast index to seq_t for arithmetic */
    seq_t n3  = N * N * N;            /* n^3 */

    seq_t best = n3;                  /* upper bound; any positive diff < n^3 */

    /* Loop over all possible x (1 ≤ x < n) */
    for (idx_t ix = 1; ix < n; ++ix)
    {
        seq_t x  = (seq_t)ix;
        seq_t x3 = x * x * x;
        if (x3 >= n3) break;          /* safety check, should never trigger */

        seq_t rem = n3 - x3;          /* remainder to be expressed as y^3 plus d */

        seq_t y = floor_cube_root(rem); /* greatest y with y^3 ≤ rem */
        if (y == 0) y = 1;            /* rem > 0 ensures y ≥ 1 */

        seq_t y3 = y * y * y;
        seq_t d  = rem - y3;          /* positive by FLT (no exact zero) */

        if (d > 0 && d < best)
        {
            best = d;
            if (best == 1) break;     /* minimal possible value */
        }
    }

    return best;
}
