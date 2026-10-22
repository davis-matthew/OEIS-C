/*
 * A330272  a(n) is the least nonnegative integer k such that n OR k is a cube
 *          (where OR denotes the bitwise OR operator).
 * https://oeis.org/A330272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that (n | k) is a perfect cube, i.e. the
 *            smallest cube m with (m & n) == n and k = m & ~n
 * Range:     u8 n <= 5, u16 n <= 61, u32 n <= 2005, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    /* Find the largest integer r with r³ ≤ s. */
    seq_t r = floor_cube_root(s);
    seq_t c = r * r * r;
    /* If n itself is a perfect cube, k = 0. */
    if (c == s) {
        return (seq_t)0;
    }
    /* Search upward for the first cube that covers all bits of n. */
    for (seq_t x = r + 1; ; ++x) {
        c = x * x * x;
        if ((c & s) == s) {
            /* The minimal k is the part of the cube not present in n. */
            return c & ~s;
        }
    }
}
