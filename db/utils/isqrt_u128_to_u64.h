/* isqrt_u128_to_u64: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_U128_TO_U64_H
#define OEISC_UTILS_ISQRT_U128_TO_U64_H
#include <stdint.h>

/* Integer square root of a 128–bit unsigned integer, floor value. */
static inline uint64_t isqrt_u128_to_u64(unsigned __int128 v)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 ans = 0;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= v) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow */
            hi  = mid - 1;
        }
    }
    return (uint64_t)ans;
}

#endif
