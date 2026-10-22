/*
 * A338162  Number of ways to write 4*n + 1 as x^2 + y^2 + z^2 + w^2 with x^2
 *          + 7*y^2 = 2^k for some k = 0,1,2,..., where x, y, z, w are
 *          nonnegative integers with z <= w.
 * https://oeis.org/A338162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the number of quadruples (x,y,z,w) of nonnegative
 * integers satisfying
 *
 *   x^2 + y^2 + z^2 + w^2 = 4*n + 1
 *   x^2 + 7*y^2 = 2^k   for some integer k ≥ 0
 *   z ≤ w
 *
 * The algorithm enumerates all (x,y) whose x^2+7*y^2 is a power of two.
 * For each such pair it computes the remaining sum R = 4*n+1 – (x^2+y^2)
 * and then counts the unordered pairs (z,w) with z^2+w^2 = R.
 *
 * The count is returned as a seq_t value.  All arithmetic uses 64‑bit
 * unsigned integers; for large indices the operations stay within 64
 * bits because 4*n+1 fits into uint64_t for the tested range.
 */

#include <stdint.h>

/* integer square root (floor) for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x >> 1) | 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Check if v is a power of two (v>0) */
static inline int is_power_of_two_u64(uint64_t v) {
    return v != 0 && (v & (v - 1)) == 0;
}

seq_t a(idx_t n)
{
    /* target value M = 4*n + 1 */
    uint64_t M = 4 * n + 1;
    seq_t ans = 0;

    /* iterate over x */
    for (uint64_t x = 0; x * x <= M; ++x) {
        uint64_t xsq = x * x;
        /* maximum y such that x^2 + y^2 <= M */
        uint64_t y_max = isqrt_u64(M - xsq);
        for (uint64_t y = 0; y <= y_max; ++y) {
            uint64_t ysq = y * y;
            uint64_t term = xsq + 7 * ysq;   /* x^2 + 7*y^2 */
            if (!is_power_of_two_u64(term)) continue;

            /* remaining sum for z and w */
            uint64_t R = M - (xsq + ysq);
            uint64_t sqrtR = isqrt_u64(R);
            for (uint64_t z = 0; z <= sqrtR; ++z) {
                uint64_t zsq = z * z;
                uint64_t wsq = R - zsq;
                uint64_t w = isqrt_u64(wsq);
                if (w * w == wsq && w >= z) {
                    ans += 1;
                }
            }
        }
    }
    return ans;
}
