/* popcount_u32_to_u32: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_U32_TO_U32_H
#define OEISC_UTILS_POPCOUNT_U32_TO_U32_H
#include <stdint.h>

static inline unsigned int popcount_u32_to_u32(uint32_t x)
{
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    return (unsigned int)__builtin_popcount(x);
#else
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
#endif
}

#endif
