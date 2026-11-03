/* icbrt_u128: floor(cbrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ICBRT_U128_H
#define OEISC_UTILS_ICBRT_U128_H
#include <stdint.h>

/* Integer floor cube‑root for 0 ≤ x < 2^128.
   Returns the largest integer r with r³ ≤ x. */
static inline uint64_t icbrt_u128(__uint128_t x)
{
    if (x == 0) return 0;
    uint64_t low  = 1;
    uint64_t high = ((uint64_t)1) << 44;   /* 2^44 > (2^128−1)^(1/3) ≈ 2^42.7 */

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;          /* mid² fits in 128 bits */
        /* test mid³ ≤ x without overflow:
           mid³ ≤ x  ⇔  mid² ≤ x / mid   (mid > 0) */
        if (mid2 <= x / mid) {
            low = mid + 1;            /* mid is a candidate, search higher */
        } else {
            high = mid - 1;           /* mid is too large, search lower */
        }
    }
    return high;   /* high is the floor of the cube root */
}

#endif
