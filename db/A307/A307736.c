/*
 * A307736  Sum of the largest sides of all acute integer triangles with
 *          perimeter n.
 * https://oeis.org/A307736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of largest sides of all acute integer triangles with
 *            perimeter n
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   ceil_half_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over the largest side c, count the valid (a,b) pairs, and add c*count to the sum. */

#include <stdint.h>
#include "../utils/ceil_half_int64.h"

static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
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
    seq_t sum = (seq_t)0;
    if (n < 3) return sum;            /* for n = 1,2 the sum is 0 */

    idx_t c_min = (n + 2) / 3;         /* ceil(n/3)   */
    idx_t c_max = (n - 1) / 2;         /* floor((n-1)/2) */

    for (idx_t c = c_min; c <= c_max; ++c) {
        idx_t S = n - c;               /* a + b */

        /* a_min = max(1, n - 2c) */
        idx_t a_min = (n > 2 * c) ? (n - 2 * c) : 1;

        /* a_max = floor(S / 2) */
        idx_t a_max = S >> 1;

        if (a_min > a_max) continue;   /* no triangles for this c */

        unsigned __int128 two_cc = (unsigned __int128)2 * c * c;
        unsigned __int128 S_sq  = (unsigned __int128)S * S;

        if (two_cc < S_sq) {           /* D < 0: all a are acute */
            idx_t count = a_max - a_min + 1;
            sum += (seq_t)c * (seq_t)count;
            continue;
        }

        /* D >= 0 */
        idx_t D = (idx_t)(two_cc - S_sq);     /* 2c^2 - S^2 */
        idx_t sqrtD = isqrt_idx(D);

        int64_t num_c   = (int64_t)S - (int64_t)sqrtD;    /* S - sqrtD */
        int64_t ceil_low = ceil_half_int64(num_c);        /* ceil((S - sqrtD)/2) */
        int64_t floor_high = ((int64_t)S + (int64_t)sqrtD) >> 1; /* floor((S + sqrtD)/2) */

        int64_t a_min_i = (int64_t)a_min;
        int64_t a_max_i = (int64_t)a_max;

        int64_t banned_low = ceil_low > a_min_i ? ceil_low : a_min_i;
        int64_t banned_high = floor_high < a_max_i ? floor_high : a_max_i;

        idx_t overlap = 0;
        if (banned_low <= banned_high)
            overlap = (idx_t)(banned_high - banned_low + 1);

        idx_t total = (idx_t)(a_max_i - a_min_i + 1);
        idx_t count = total - overlap;
        sum += (seq_t)c * (seq_t)count;
    }

    return sum;
}
