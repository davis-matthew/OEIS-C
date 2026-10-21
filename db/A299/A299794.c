/*
 * A299794  Number of ways to write n^2 as x^2 + y^2 + z^2 + w^2 with x >= y
 *          >= 0 <= z <= w such that x or 2*y is a power of 4 (including 4^0 =
 *          1) and x + 15*y is also a power of 4.
 * https://oeis.org/A299794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n^2 as x^2 + y^2 + z^2 + w^2 with
 *            x >= y >= 0 <= z <= w, x or 2*y is a power of 4, and x + 15*y is
 *            a power of 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power4_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_power4_idx.h"

/* Integer square root for idx_t (unsigned 64‑bit). */
static inline idx_t int_sqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t N = n * n;                      /* n^2 */
    idx_t max_sum = 16 * n;                /* upper bound for x + 15*y */

    for (idx_t y = 0; y <= n; ++y) {
        idx_t twoy = 2 * y;
        bool y_power = is_power4_idx(twoy);        /* 2*y is a power of 4? */

        /* Iterate over possible powers of 4 for x + 15*y. */
        for (idx_t sum = 1; sum <= max_sum; sum <<= 2) {
            if (sum < 15 * y) continue;           /* would make x negative */

            idx_t x = sum - 15 * y;
            if (x < y || x > n) continue;         /* ordering or bound fail */

            /* Check x^2 + y^2 <= n^2 */
            unsigned __int128 xsq = (unsigned __int128)x * x;
            unsigned __int128 ysq = (unsigned __int128)y * y;
            if (xsq + ysq > N) continue;

            /* At least one of the conditions must hold. */
            if (! (is_power4_idx(x) || y_power)) continue;

            /* R = n^2 - x^2 - y^2 */
            idx_t R = N - (idx_t)xsq - (idx_t)ysq;
            idx_t sqrtR = int_sqrt_idx(R);

            /* Count representations R = z^2 + w^2 with 0 <= z <= w. */
            for (idx_t z = 0; z <= sqrtR; ++z) {
                idx_t z2 = z * z;
                idx_t rem = R - z2;
                idx_t w = int_sqrt_idx(rem);
                if (w * w == rem && w >= z) {
                    ++result;
                }
            }
        }
    }

    return result;
}
