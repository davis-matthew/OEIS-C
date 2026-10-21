/*
 * A283269  Number of ways to write n as x^2 + y^2 + z^2 with x,y,z integers
 *          such that x + 3*y + 5*z is a square.
 * https://oeis.org/A283269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) such that * n = x^2 +
 *            y^2 + z^2 * and x + 3*y + 5*z is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for 64‑bit unsigned values   */
/* returns floor(sqrt(v))                           */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

static inline bool is_square_u64(idx_t v)
{
    idx_t r = isqrt_u64(v);
    return (__uint128_t)r * r == v;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* maximum absolute value of x or y needed */
    idx_t max_xy = isqrt_u64(n);

    for (int64_t x = -(int64_t)max_xy; x <= (int64_t)max_xy; ++x) {
        idx_t x2 = (idx_t)((__int128_t)x * x);          /* x^2  */
        if (x2 > n) continue;

        idx_t rem_after_x = n - x2;
        idx_t max_y = isqrt_u64(rem_after_x);

        for (int64_t y = -(int64_t)max_y; y <= (int64_t)max_y; ++y) {
            idx_t y2 = (idx_t)((__int128_t)y * y);      /* y^2  */
            idx_t sum_xy = x2 + y2;
            if (sum_xy > n) continue;

            idx_t z2 = n - sum_xy;
            if (!is_square_u64(z2)) continue;          /* z^2 must be a square */

            idx_t z_abs = isqrt_u64(z2);

            /* Consider z = +z_abs and –z_abs (only once if z_abs == 0) */
            for (int s = 0; s < 2; ++s) {
                if (z_abs == 0 && s == 1) break;
                int64_t z = s ? -(int64_t)z_abs : (int64_t)z_abs;

                int64_t Lval = x + 3 * y + 5 * z;
                if (Lval < 0) continue;

                idx_t L = (idx_t)Lval;
                if (is_square_u64(L))
                    count++;
            }
        }
    }
    return count;
}
