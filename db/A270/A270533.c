/*
 * A270533  Number of ordered ways to write n = x^4 + x^3 + y^2 + z*(3z-1)/2,
 *          where x and y are nonnegative integers, and z is an integer.
 * https://oeis.org/A270533
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

/* Count of representations of n as x^4 + x^3 + y^2 + z*(3z-1)/2
 * The algorithm iterates over all non‑negative integers x and y
 * with x^4 + x^3 + y^2 ≤ n and checks if the remaining value
 * can be written as z*(3z-1)/2 for some integer z.
 * The check uses the quadratic equation: 3z^2 - z - 2R = 0.
 * For a given residual R, an integer solution exists iff
 * 1 + 24R is a perfect square d^2 and d ≡ 1 or 5 (mod 6).
 */
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for idx_t using binary search.  */
static inline idx_t
isqrt_idx(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;   /* 2^32 > sqrt(2^64) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;   /* high^2 ≤ n < (high+1)^2 */
}

/* Count representations for a given n. */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* iterate over x */
    for (idx_t x = 0; ; ++x) {
        /* compute x^4 and x^3 in standard integers */
        uint64_t x64 = (uint64_t)x;
        uint64_t x4 = x64 * x64 * x64 * x64;
        uint64_t x3 = x64 * x64 * x64;
        uint64_t sum_x = x4 + x3;
        if (sum_x > (uint64_t)n) break;

        /* iterate over y */
        for (idx_t y = 0; ; ++y) {
            uint64_t y64 = (uint64_t)y;
            uint64_t y2 = y64 * y64;
            uint64_t total_xy = sum_x + y2;
            if (total_xy > (uint64_t)n) break;

            uint64_t t = (uint64_t)n - total_xy;   /* residual for z part */
            if (t == 0) {
                /* z = 0 is a valid solution */
                count++;
                continue;
            }

            /* Solve 3z^2 - z - 2t = 0  ->  D = 1 + 24t */
            uint64_t D = 1 + 24 * t;
            idx_t d = isqrt_idx(D);
            if ((uint64_t)d * (uint64_t)d != D) continue;           /* not a perfect square */

            if (d % 6 == 1 || d % 6 == 5) {   /* integer z exists */
                count++;
            }
        }
    }
    return count;
}
