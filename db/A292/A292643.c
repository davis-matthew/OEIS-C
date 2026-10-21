/*
 * A292643  Rank of (5+r)*n when all the numbers (5-r)*j and (5+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor( (27 + 10√2) * n / 23 ) Derived from (5 - √2) j
 *            ≤ (5 + √2) n ⇔ 23 j² – 54 n j + 23 n² ≤ 0
 * Range:     u8 n <= 91, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Binary search for the largest j ≥ 1 satisfying 23 j² – 54 n j + 23 n² ≤ 0. */
    idx_t low = 1;
    __int128 n128 = (__int128)n;

    /* Upper bound 2 n (safe, may overflow – clamp to UINT64_MAX). */
    __int128 high128 = n128 * 2;
    idx_t high = (high128 > ((__int128)(~(idx_t)0))) ? ~(idx_t)0 : (idx_t)high128;

    idx_t jmax = 0;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 m = (__int128)mid;
        __int128 f = 23 * m * m - 54 * n128 * m + 23 * n128 * n128;

        if (f <= 0) {
            jmax = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;            /* defensive guard */
            high = mid - 1;
        }
    }

    return (seq_t)((__int128)n + (__int128)jmax);
}
