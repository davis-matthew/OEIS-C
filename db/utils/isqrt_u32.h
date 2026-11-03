/* isqrt_u32: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_U32_H
#define OEISC_UTILS_ISQRT_U32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Integer square root for 32‑bit values ---------- */
static inline uint32_t isqrt_u32(uint32_t n)
{
    uint32_t x = n;
    uint32_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

#endif
