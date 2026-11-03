/* ctz_u32: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_U32_H
#define OEISC_UTILS_CTZ_U32_H
#include <stdbool.h>
#include <stdint.h>

/* Count bits with lowest set bit index (0‑based).  GCC built‑in used if available. */
static int ctz_u32(uint32_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctz(x);
#else
    int i = 0;
    while ((x & 1) == 0) { x >>= 1; ++i; }
    return i;
#endif
}

#endif
