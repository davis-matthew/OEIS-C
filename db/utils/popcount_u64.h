/* popcount_u64: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_U64_H
#define OEISC_UTILS_POPCOUNT_U64_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* fast popcount: use the compiler built‑in if available */
static inline unsigned int popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

#endif
