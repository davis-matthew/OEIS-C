/*
 * A300908  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with (3*x)^2 +
 *          (4*y)^2 + (12*z)^2 a square , where w is a positive integer and
 *          x,y,z are nonnegative integers such that z or 2*z or 3*z is a
 *          square.
 * https://oeis.org/A300908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = x^2 + y^2 + z^2 + w^2 with
 *            x,y,z ≥ 0, w ≥ 1, (3x)^2 + (4y)^2 + (12z)^2 a square, and one of
 *            z, 2z, 3z a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(v))   v is unsigned 64‑bit */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > v / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* test whether v is a perfect square */
static inline bool is_square(idx_t v)
{
    idx_t r = isqrt_u64(v);
    return r * r == v;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset begins at 1 */

    /* maximum possible value for x, y, z so that w² ≥ 1 */
    idx_t lim = isqrt_u64(n - 1);

    seq_t count = 0;

    for (idx_t x = 0; x <= lim; ++x) {
        idx_t x2 = x * x;
        if (x2 >= n) break;                 /* no room for y, z, w */

        for (idx_t y = 0; y <= lim; ++y) {
            idx_t y2 = y * y;
            idx_t sum_xy = x2 + y2;
            if (sum_xy >= n) break;          /* no room for z, w */

            for (idx_t z = 0; z <= lim; ++z) {
                idx_t z2 = z * z;
                idx_t sum_xyz = sum_xy + z2;
                if (sum_xyz >= n) break;      /* w² would be zero */

                /* w² = n - sum_xyz, must be a perfect square */
                idx_t r = n - sum_xyz;
                if (r == 0) continue;         /* w must be positive */
                idx_t w = isqrt_u64(r);
                if (w * w != r) continue;

                /* z or 2z or 3z must be a perfect square */
                if (!(is_square(z) || is_square(2 * z) || is_square(3 * z)))
                    continue;

                /* (3x)² + (4y)² + (12z)² must be a perfect square */
                idx_t s_val = 9 * x2 + 16 * y2 + 144 * z2;
                if (!is_square(s_val)) continue;

                ++count;                       /* valid representation found */
            }
        }
    }

    return count;
}
