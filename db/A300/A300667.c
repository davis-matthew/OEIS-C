/*
 * A300667  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers and z <= w such that 3*x or y is a square and
 *          x + 2*y is also a square.
 * https://oeis.org/A300667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (x,y,z,w) | x,y,z,w ∈ ℕ₀, x² + y² + z² + w² = n, z ≤
 *            w, (3·x or y) is a square, x + 2·y is a square }| The
 *            implementation loops over the possible x, y and z values and
 *            checks the remaining conditions. For the small range (n ≤ 10000)
 *            this runs easily within the required time limit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root – floor of sqrt(n) for idx_t values */
static inline idx_t isqrt_idx_t(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) {
            return mid;
        } else if (sq < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;          /* floor(sqrt(n)) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t maxRoot = isqrt_idx_t(n);
    seq_t count = 0;

    for (idx_t x = 0; x <= maxRoot; ++x) {
        idx_t xsq = x * x;

        /* 3*x must be a square or y must be a square.  We test that inside the y loop. */
        for (idx_t y = 0; y <= maxRoot; ++y) {
            idx_t sum_xy = xsq + y * y;
            if (sum_xy > n)
                break; /* further y only increase sum_xy */

            /* Check 3*x is a square */
            idx_t sq3x = isqrt_idx_t(3 * x);
            bool cond3x = (sq3x * sq3x == 3 * x);

            /* Check y is a square */
            idx_t sqy   = isqrt_idx_t(y);
            bool condy  = (sqy * sqy == y);

            if (!(cond3x || condy))
                continue; /* skip this (x,y) pair */

            /* Check x + 2*y is a square */
            idx_t s = x + 2 * y;
            idx_t sqs = isqrt_idx_t(s);
            if (sqs * sqs != s)
                continue;

            /* Now iterate over z */
            idx_t remainingAfterXY = n - sum_xy;
            idx_t maxZ = isqrt_idx_t(remainingAfterXY);
            for (idx_t z = 0; z <= maxZ; ++z) {
                idx_t sum_xyz = sum_xy + z * z;
                if (sum_xyz > n)
                    break; /* further z only increase sum_xyz */

                idx_t rem = n - sum_xyz;
                idx_t w = isqrt_idx_t(rem);
                if (w * w != rem)
                    continue;

                if (w < z)
                    continue;

                /* All conditions satisfied */
                ++count;
            }
        }
    }

    return count;
}
