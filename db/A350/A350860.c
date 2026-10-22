/*
 * A350860  Number of ways to write n as w^4 + (x^4 + y^2 + z^2)/81, where
 *          w,x,y,z are nonnegative integers with y <= z.
 * https://oeis.org/A350860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 3761, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A350860: a(n) = number of non‑negative integer solutions 
 * (w,x,y,z) to 
 *     n = w^4 + (x^4 + y^2 + z^2) / 81    with    y ≤ z.
 * 
 * The implementation performs a brute‑force search over w,x,y,z.  
 * Intermediate calculations are done in idx_t (uint64_t) to avoid
 * overflow; the final count is accumulated in seq_t.
 * 
 * An integer square‑root helper for idx_t values is implemented below.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values, truncating down. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t w = 0; ; ++w) {
        idx_t w4 = (idx_t)w * w * w * w;
        if (w4 > n) break;

        /* remaining part: x^4 + y^2 + z^2 = 81 * (n - w^4) */
        idx_t diff   = n - w4;
        idx_t rem81  = diff * 81ULL;          /* 81*(n - w^4) */

        for (idx_t x = 0; ; ++x) {
            idx_t x4 = (idx_t)x * x * x * x;
            if (x4 > rem81) break;

            idx_t k = rem81 - x4;            /* y^2 + z^2 must equal k */

            for (idx_t y = 0; ; ++y) {
                idx_t y2 = y * y;
                if (y2 > k) break;

                idx_t z2 = k - y2;           /* z^2 must equal z2 */

                if (z2 < y2) break;          /* ensures y ≤ z */

                idx_t z = isqrt_u64(z2);
                if (z * z == z2) {
                    result += (seq_t)1;
                }
            }
        }
    }

    return result;
}
