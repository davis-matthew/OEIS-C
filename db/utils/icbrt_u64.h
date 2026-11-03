/* icbrt_u64: floor(cbrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ICBRT_U64_H
#define OEISC_UTILS_ICBRT_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: integer cube root (floor) for uint64_t.  The value fits in 64 bits
   for all numbers encountered by this algorithm. */
static uint64_t icbrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 1;
    /* Grow hi until hi^3 > n. */
    while ((unsigned __int128)hi * hi * hi <= n) hi <<= 1;
    /* Binary search for floor(cuberoot(n)). */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube <= n) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

#endif
