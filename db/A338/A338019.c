/*
 * A338019  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with 3*x + 10*y
 *          + 36*z a positive square, where x, y, z, w are nonnegative
 *          integers.
 * https://oeis.org/A338019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: count all tuples (x,y,z,w) of non‑negative integers with
   x^2 + y^2 + z^2 + w^2 = n and 3x + 10y + 36z a positive square.  */

#include <stdbool.h>

/* integer square root: floor(sqrt(n)) for unsigned 64‑bit values. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* n is 1‑based in the OEIS sequence definition */
    if (n == 0) return 0;

    seq_t count = 0;
    idx_t max_xy = isqrt_u64(n);          /* maximum possible value for x,y,z */

    for (idx_t x = 0; x <= max_xy; ++x) {
        idx_t x2 = x * x;
        if (x2 > n) break;

        for (idx_t y = 0; y <= isqrt_u64(n - x2); ++y) {
            idx_t y2 = y * y;
            idx_t sum_xy = x2 + y2;
            if (sum_xy > n) break;

            for (idx_t z = 0; z <= isqrt_u64(n - sum_xy); ++z) {
                idx_t z2 = z * z;
                idx_t sum_xyz = sum_xy + z2;
                if (sum_xyz > n) break;

                idx_t w2 = n - sum_xyz;
                idx_t w = isqrt_u64(w2);
                if ((unsigned __int128)w * w != w2) continue;  /* w must be integer */

                /* linear form 3x + 10y + 36z must be a positive square */
                idx_t val = 3ULL * x + 10ULL * y + 36ULL * z;
                if (val == 0) continue;
                idx_t s = isqrt_u64(val);
                if ((unsigned __int128)s * s == val)
                    ++count;
            }
        }
    }
    return count;
}
