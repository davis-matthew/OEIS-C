/* isqrt_ceil: ceil(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_CEIL_H
#define OEISC_UTILS_ISQRT_CEIL_H
#include <stdint.h>

static inline idx_t isqrt_ceil(idx_t n)
{
    /* binary search for floor(sqrt(n)), then take ceiling */
    idx_t low  = 0;
    idx_t high = (idx_t)1ULL << 32;          /* >= sqrt(UINT64_MAX) */

    /* Find maximal m with m*m <= n without overflow */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {            /* avoid division by zero */
            low = 1;
            continue;
        }
        if (mid <= n / mid)        /* mid*mid <= n */
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t floor = high;             /* floor(sqrt(n)) */

    if (floor * floor < n)         /* ceil needed if not exact */
        return floor + 1;
    return floor;                   /* exact square */
}

#endif
