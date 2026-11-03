/* isqrt_u64: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_U64_H
#define OEISC_UTILS_ISQRT_U64_H
#include <stdint.h>

static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

#endif
