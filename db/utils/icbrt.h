/* icbrt: floor(cbrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ICBRT_H
#define OEISC_UTILS_ICBRT_H
#include <stdint.h>

static idx_t icbrt(idx_t x)
{
    /* Binary search for the largest integer r such that r^3 <= x.
       x is <= UINT64_MAX, so r <= 1<<22 (≈4M). */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 22;          /* high^3 > 0 and > maximum x */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;    /* upper half */
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= (__int128)x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

#endif
