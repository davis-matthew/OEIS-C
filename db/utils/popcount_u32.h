/* popcount_u32: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_U32_H
#define OEISC_UTILS_POPCOUNT_U32_H
#include <stdint.h>
#include <stdlib.h>

/* Popcount for a small unsigned integer. */
static inline int popcount_u32(unsigned int x)
{
    int c = 0;
    while (x)
    {
        c += x & 1U;
        x >>= 1U;
    }
    return c;
}

#endif
