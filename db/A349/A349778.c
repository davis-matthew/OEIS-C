/*
 * A349778  Number of ways to write n as x^2 + y^k + 2*z^m, where x,y,z are
 *          nonnegative integers with x >= y, and k and m belong to the set
 *          {2,3}.
 * https://oeis.org/A349778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as x^2 + y^k + 2*z^m with
 *            x, y, z ≥ 0 integers, x ≥ y, k ∈ {2,3} and m ∈ {2,3}
 * Range:     u8 n <= 1905, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_root_u64, is_cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cube_root_u64.h"
#include "../utils/is_cube.h"

/* integer square root (floor) for unsigned 64‑bit values */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* (2^32)^2 = 2^64 > max idx_t */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > n / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

/* Test if t is a perfect square; returns true if yes, false otherwise. */
static inline bool is_square(idx_t t)
{
    idx_t r = floor_sqrt_u64(t);
    return (seq_t)r * (seq_t)r == (seq_t)t;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* largest possible x with x^2 ≤ n */
    idx_t xmax = floor_sqrt_u64(n);
    for (idx_t x = 0; x <= xmax; ++x) {
        idx_t rem1 = n - x * x;          /* remaining after x^2 */

        /* k = 2 : y^2 term */
        idx_t ymax_sq = floor_sqrt_u64(rem1);
        for (idx_t y = 0; y <= ymax_sq && y <= x; ++y) {
            idx_t rem2 = rem1 - y * y;
            if (rem2 & 1) continue;     /* 2*z^m must be even */
            idx_t t = rem2 >> 1;        /* t = z^m */

            if (is_square(t)) ++count;  /* m = 2 */
            if (is_cube(t))   ++count;  /* m = 3 */
        }

        /* k = 3 : y^3 term */
        idx_t ymax_cb = cube_root_u64(rem1);
        for (idx_t y = 0; y <= ymax_cb && y <= x; ++y) {
            idx_t rem2 = rem1 - y * y * y;
            if (rem2 & 1) continue;
            idx_t t = rem2 >> 1;

            if (is_square(t)) ++count;
            if (is_cube(t))   ++count;
        }
    }

    return count;
}
