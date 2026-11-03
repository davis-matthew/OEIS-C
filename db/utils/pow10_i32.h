/* pow10_i32: 10^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW10_I32_H
#define OEISC_UTILS_POW10_I32_H
#include <stdint.h>

/* Compute 10^e in 128‑bit arithmetic (e <= 38 for all n up to 10,000) */
static inline __uint128_t pow10_i32(int e)
{
    __uint128_t r = 1;
    for (int i = 0; i < e; ++i)
        r *= 10;
    return r;
}

#endif
