/* isqrt_i128: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_I128_H
#define OEISC_UTILS_ISQRT_I128_H
#include <stdbool.h>
#include <stdint.h>

/* Integer square root for a 128‑bit positive value.
   Returns floor( sqrt(v) ). */
static uint64_t isqrt_i128(__int128 v)
{
    __int128 lo = 0, hi = 1;
    /* enlarge hi until hi² > v */
    while (hi * hi <= v) hi <<= 1;
    while (lo + 1 < hi) {
        __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= v) lo = mid;
        else hi = mid;
    }
    return (uint64_t)lo;
}

#endif
