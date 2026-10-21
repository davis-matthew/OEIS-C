/*
 * A295976  Number of nonnegative solutions to (x,y) = 1 and x^3 + y^3 = n.
 * https://oeis.org/A295976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y) ∈ ℕ₀² : gcd(x,y)=1, x³ + y³ = n}|
 * Range:     u8 n <= 63, u16 n <= 9002, u32 n <= 9002, u64 n <= 9002,
 *            u128 n <= 9002, big any n
 * Helpers:   floor_cube_root, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* cube root of n as an integer */
    idx_t croot = (idx_t)floor_cube_root((seq_t)n);

    seq_t ans = 0;

    for (idx_t x = 0; x <= croot; ++x)
    {
        seq_t x_cube = (seq_t)x * (seq_t)x * (seq_t)x;
        seq_t y_cube_rem = (seq_t)n - x_cube;

        /* y is the integer cube root of the remaining part */
        idx_t y = (idx_t)floor_cube_root(y_cube_rem);
        seq_t y_cube = (seq_t)y * (seq_t)y * (seq_t)y;

        if (y_cube != y_cube_rem)
            continue;          /* not a perfect cube, skip */

        /* count only coprime pairs */
        if (gcd((seq_t)x, (seq_t)y) == 1)
            ++ans;
    }

    return ans;
}
