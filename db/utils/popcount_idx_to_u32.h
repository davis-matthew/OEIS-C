/* popcount_idx_to_u32: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_IDX_TO_U32_H
#define OEISC_UTILS_POPCOUNT_IDX_TO_U32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------
   Popcount of an idx_t value (works for 32/64/128‑bit unsigned types).
   ---------------------------------------------------------------------- */
static inline unsigned int popcount_idx_to_u32(idx_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

#endif
