/* isqrt_i64: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_I64_H
#define OEISC_UTILS_ISQRT_I64_H
#include <stddef.h>
#include <stdint.h>

/* Integer square–root for signed 64‑bit values.  Returns floor(sqrt(v)). */
static long long isqrt_i64(long long v)
{
    long long lo = 0;
    long long hi = v < 3037000499LL ? v : 3037000499LL;  /* sqrt(LLONG_MAX) */
    while (lo < hi) {
        long long mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

#endif
