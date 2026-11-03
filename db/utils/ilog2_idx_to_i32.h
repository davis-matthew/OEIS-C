/* ilog2_idx_to_i32: floor(log2(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG2_IDX_TO_I32_H
#define OEISC_UTILS_ILOG2_IDX_TO_I32_H
#include <stdint.h>

static int ilog2_idx_to_i32(idx_t n)
{
    int pos = 0;
    /* n > 0 when this function is called. */
    while ((n >> 1) != 0)
    {
        n >>= 1;
        pos++;
    }
    return pos;                 /* most‑significant bit index (0‑based) */
}

#endif
