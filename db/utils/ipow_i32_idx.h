/* ipow_i32_idx: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_I32_IDX_H
#define OEISC_UTILS_IPOW_I32_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* Helper: exponentiation with 128‑bit signed integer (fast). */
static inline __int128 ipow_i32_idx(int base, idx_t exp)
{
    __int128 result = 1;
    __int128 b = base;
    while (exp)
    {
        if (exp & 1U) result *= b;
        b *= b;
        exp >>= 1U;
    }
    return result;
}

#endif
