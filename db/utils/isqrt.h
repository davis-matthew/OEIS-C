/* isqrt: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_H
#define OEISC_UTILS_ISQRT_H
#include <stdint.h>

/* Binary search avoiding overflow */
static inline idx_t isqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1, hi = n >> 1;            /* hi < sqrt(max) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (mid > n / mid)                /* mid*mid > n   -> overflow or too big */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

#endif
