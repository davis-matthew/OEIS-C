/*
 * A271237  Number of ordered ways to write n as u^3 + 2*v^3 + 3*x^3 + 4*y^3 +
 *          5*z^3, where u, v, x, y and z are nonnegative integers.
 * https://oeis.org/A271237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (u,v,x,y,z) ∈ ℕ⁵ : u³ + 2v³ + 3x³ + 4y³ + 5z³ = n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation performs an exhaustive search over the bounded ranges
   for each variable. The ranges are derived from the cube roots of the
   remaining target after processing the preceding variables. */

#include <stddef.h>       /* size_t */
#include <stdint.h>      /* uint64_t */
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* Accumulator of solutions. Use a wide type for safety; the final value
       is cast to seq_t, which may be of any size (uint8_t … arbitrary). */
    unsigned long long solutions = 0ULL;

    /* u variable: coefficient 1 */
    idx_t max_u = int_cube_root(n);
    for (idx_t u = 0; u <= max_u; ++u) {
        uint64_t u3 = (uint64_t)u * u * u;
        if (u3 > n) break;
        uint64_t rem_u = n - u3;

        /* v variable: coefficient 2 */
        idx_t max_v = int_cube_root(rem_u / 2);
        for (idx_t v = 0; v <= max_v; ++v) {
            uint64_t v3 = (uint64_t)v * v * v;
            uint64_t two_v3 = 2ULL * v3;
            if (two_v3 > rem_u) continue;
            uint64_t rem_uv = rem_u - two_v3;

            /* x variable: coefficient 3 */
            idx_t max_x = int_cube_root(rem_uv / 3);
            for (idx_t x = 0; x <= max_x; ++x) {
                uint64_t x3 = (uint64_t)x * x * x;
                uint64_t three_x3 = 3ULL * x3;
                if (three_x3 > rem_uv) continue;
                uint64_t rem_uvx = rem_uv - three_x3;

                /* y variable: coefficient 4 */
                idx_t max_y = int_cube_root(rem_uvx / 4);
                for (idx_t y = 0; y <= max_y; ++y) {
                    uint64_t y3 = (uint64_t)y * y * y;
                    uint64_t four_y3 = 4ULL * y3;
                    if (four_y3 > rem_uvx) continue;
                    uint64_t rem_uvxy = rem_uvx - four_y3;

                    /* z variable: coefficient 5 */
                    if (rem_uvxy % 5ULL != 0) continue;
                    uint64_t z3 = rem_uvxy / 5ULL;
                    uint64_t z = int_cube_root(z3);
                    if (z * z * z == z3) {
                        ++solutions;
                    }
                }
            }
        }
    }

    return (seq_t)solutions;
}
