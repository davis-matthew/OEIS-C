/* ipow_u32_idx: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U32_IDX_H
#define OEISC_UTILS_IPOW_U32_IDX_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* compute power p^e (p is small, e is idx_t).  Works for any seq_t. */
static inline seq_t ipow_u32_idx(uint32_t p, idx_t e)
{
    seq_t result = 1;
    seq_t base   = p;
    while (e) {
        if (e & 1) result *= base;
        e >>= 1;
        if (e) base *= base;
    }
    return result;
}

#endif
