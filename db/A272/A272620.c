/*
 * A272620  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with w
 *          + x + y - z a square, where w is an integer and x,y,z are
 *          nonnegative integers with |w| <= x >= y <= z < x + y.
 * https://oeis.org/A272620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to write n as w^2 + x^2 + y^2 +
 *            z^2 with |w| <= x, x >= y, y <= z < x + y, and (w + x + y - z)
 *            is a perfect square. This function implements that definition
 * Range:     u8 n <= 5417, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square root for unsigned 64‑bit numbers.  */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t lo = 1, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t div = n / mid;
        if (mid > div) {
            /* mid*mid > n */
            hi = mid - 1;
        } else {
            /* mid*mid <= n */
            if ((mid + 1) > n / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* not defined for n < 1 */

    uint64_t N     = n;                  /* the input as 64‑bit */
    seq_t     count = (seq_t)0;          /* result accumulator */

    uint64_t max_xy = isqrt_u64(N);      /* max possible value for x (and y) */

    for (uint64_t x = 0; x <= max_xy; ++x) {
        uint64_t xsq = x * x;
        if (xsq > N) break;

        for (uint64_t y = 0; y <= x; ++y) {
            uint64_t ysq = y * y;
            uint64_t xyss = xsq + ysq;
            if (xyss > N) break;

            uint64_t rem_xy = N - xyss;

            /* z^2 must not exceed rem_xy */
            uint64_t max_z_sq = isqrt_u64(rem_xy);

            /* z must satisfy y <= z < x + y */
            uint64_t z_limit = (x + y == 0) ? 0 : (x + y - 1);
            if (z_limit > max_z_sq) z_limit = max_z_sq;

            for (uint64_t z = y; z <= z_limit; ++z) {
                uint64_t zsq = z * z;
                uint64_t rem = rem_xy - zsq;

                uint64_t k = isqrt_u64(rem);
                if (k * k != rem) continue;          /* w^2 must be a perfect square */

                if (k > x) continue;                  /* |w| <= x */

                /* w = +k */
                int64_t s1 = (int64_t)k + (int64_t)x + (int64_t)y - (int64_t)z;
                if (s1 >= 0) {
                    uint64_t s1u = (uint64_t)s1;
                    uint64_t s1root = isqrt_u64(s1u);
                    if (s1root * s1root == s1u)
                        ++count;
                }

                /* w = -k (if k != 0) */
                if (k != 0) {
                    int64_t s2 = -(int64_t)k + (int64_t)x + (int64_t)y - (int64_t)z;
                    if (s2 >= 0) {
                        uint64_t s2u = (uint64_t)s2;
                        uint64_t s2root = isqrt_u64(s2u);
                        if (s2root * s2root == s2u)
                            ++count;
                    }
                }
            }
        }
    }

    return count;
}
