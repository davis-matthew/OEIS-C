/*
 * A300751  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x + 3*y +
 *          5*z a positive square, where x,y,z,w are nonnegative integers such
 *          that 2*x or y or z is a square.
 * https://oeis.org/A300751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of quadruples (x,y,z,w) of nonnegative integers such
 *            that x^2 + y^2 + z^2 + w^2 = n, x + 3*y + 5*z is a positive
 *            square, and at least one of 2*x, y, z is a square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* --------- integer square root and square test --------- */
static inline idx_t floor_isqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) return 0;
        if (mid <= n / mid) {
            /* mid is candidate */
            if (mid == n / mid) return mid;          /* exact */
            if (mid + 1 > n / (mid + 1)) return mid; /* mid^2 <= n < (mid+1)^2 */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

static inline bool is_square(idx_t v)
{
    idx_t r = floor_isqrt(v);
    return r * r == v;
}

/* --------- main sequence function --------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not defined for n=0, but safe */
    idx_t limit = floor_isqrt(n);        /* maximum x,y,z (and also w) */
    seq_t count = 0;

    idx_t x;
    for (x = 0; x <= limit; ++x) {
        idx_t sx = x * x;
        if (sx > n) break;

        /* precompute whether 2*x is a square */
        bool two_x_sq = is_square(2 * x);

        idx_t y;
        for (y = 0; y <= limit; ++y) {
            idx_t sy = y * y;
            idx_t sxy = sx + sy;
            if (sxy > n) break;

            /* y is square? */
            bool y_sq = is_square(y);

            idx_t z;
            for (z = 0; z <= limit; ++z) {
                idx_t sz = z * z;
                idx_t sum = sxy + sz;
                if (sum > n) break;

                /* z is square? */
                bool z_sq = is_square(z);

                /* w^2 = n - sum must be a perfect square */
                idx_t rem = n - sum;
                idx_t w = floor_isqrt(rem);
                if (w * w != rem) continue;  /* w is integer */

                /* condition 1: x + 3*y + 5*z is a positive square */
                idx_t val = x + 3 * y + 5 * z;
                if (val == 0) continue;          /* requires positive */
                if (!is_square(val)) continue;

                /* condition 2: at least one of 2*x, y, z is a square */
                if (!two_x_sq && !y_sq && !z_sq) continue;

                ++count;
            }
        }
    }
    return count;
}
