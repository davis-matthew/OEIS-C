/* isqrt_i128_to_idx: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_I128_TO_IDX_H
#define OEISC_UTILS_ISQRT_I128_TO_IDX_H
#include <stdint.h>

/* Helper: integer square root of a 128‑bit number (floor). */
static inline idx_t isqrt_i128_to_idx(__int128 x)
{
    idx_t low = 0, high = (uint64_t)~0ULL;          /* max 64‑bit value */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            if (sq == x) return mid;
            low = mid + 1;
        } else {
            if (mid == 0) return 0;               /* never reached for x>0 */
            high = mid - 1;
        }
    }
    return high;                                    /* floor(sqrt(x)) */
}

#endif
