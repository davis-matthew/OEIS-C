/*
 * A387220  Arithmetic mean of number of lattice points strictly inside circle
 *          of radius n centered on origin, and number of points not outside
 *          that circle.
 * https://oeis.org/A387220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (A051132(n) + A000328(n)) / 2
   Arithmetic mean of lattice points strictly inside the circle of
   radius n and those not outside it. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    seq_t n2 = (seq_t)n * (seq_t)n; /* n² */
    seq_t total     = 0;            /* points with x² + y² ≤ n² */
    seq_t boundary  = 0;            /* points with x² + y² = n² */

    for (idx_t x = 0; x <= n; ++x)
    {
        seq_t xx = (seq_t)x;
        seq_t S  = n2 - xx * xx;           /* remaining square */
        seq_t r  = floor_sqrt(S);          /* ⌊√S⌋ */

        /* all lattice points for this x */
        if (x == 0)
            total += 2 * r + 1;            /* y = -r … r */
        else
            total += 4 * r + 2;            /* ±x, y = -r … r */

        /* boundary points only when r² == S */
        if (r * r == S)
        {
            if (x == 0 || r == 0)
                boundary += 2;            /* (0,±r) or (±x,0) */
            else
                boundary += 4;            /* (±x,±r) */
        }
    }

    return total - boundary / 2;
}
