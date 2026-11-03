/* ilog2_u64_to_u32: floor(log2(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG2_U64_TO_U32_H
#define OEISC_UTILS_ILOG2_U64_TO_U32_H
#include <stdint.h>

/* Integer floor‑log₂ for 64‑bit values */
static inline unsigned int ilog2_u64_to_u32(uint64_t x)
{
    unsigned int r = 0;
    while (x >>= 1) ++r;
    return r;
}

#endif
