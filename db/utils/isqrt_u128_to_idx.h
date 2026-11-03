/* isqrt_u128_to_idx: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_U128_TO_IDX_H
#define OEISC_UTILS_ISQRT_U128_TO_IDX_H
#include <stdint.h>

/* Convert a 64-bit integer to integer sqrt of a 128-bit value. */
static idx_t isqrt_u128_to_idx(__uint128_t v) {
    __uint128_t low = 0, high = (( (__uint128_t)1) << 64) - 1;
    while (low <= high) {
        __uint128_t mid = (low + high) >> 1;
        __uint128_t mid2 = mid * mid;
        if (mid2 == v) return (idx_t)mid;
        if (mid2 < v) low = mid + 1;
        else high = mid - 1;
    }
    return (idx_t)high;
}

#endif
