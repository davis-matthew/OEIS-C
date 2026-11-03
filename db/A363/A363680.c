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
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/totient.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t count = 0;
    for (;;) {
        seq_t r = floor_cube_root(x);
        if (r * r * r == x) {
            return count;
        }
        x = totient((idx_t)x);
        count++;
    }
}
