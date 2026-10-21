/*
 * A295977  Number of nonnegative solutions to (x,y) = 1 and x^3 + y^3 <= n.
 * https://oeis.org/A295977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Count coprime ordered pairs (x,y) with
   x^3 + y^3 <= n, x,y >= 0, excluding (0,0).
   Brute‑force enumeration of all x,y up to the cube root of n. */
#include "../utils/int_cube_root.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_xy = int_cube_root(n);     /* floor cube root of n */

    for (idx_t x = 0; x <= max_xy; ++x)
    {
        seq_t x3 = (seq_t)x * (seq_t)x * (seq_t)x;
        idx_t rem  = n - (idx_t)x3;     /* remaining value for y^3 */
        idx_t max_y = int_cube_root(rem);

        for (idx_t y = 0; y <= max_y; ++y)
        {
            if (x == 0 && y == 0) continue;  /* skip (0,0) */
            if (gcd((seq_t)x, (seq_t)y) == 1) ++count;
        }
    }
    return count;
}
