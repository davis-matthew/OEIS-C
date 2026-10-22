/*
 * A363680  Number of iterations of phi(x) at n needed to reach a cube.
 * https://oeis.org/A363680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that φ⁽ᵏ⁾(n) is a perfect cube, with
 *            a(1)=0
 * Range:     u8 n <= 63, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_cube_root, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t count = 0;
    for (;;) {
        seq_t r = floor_cube_root(x);
        if (r * r * r == x) {
            return count;
        }
        x = phi((idx_t)x);
        count++;
    }
}
