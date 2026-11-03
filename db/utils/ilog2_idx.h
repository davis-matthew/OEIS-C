/* ilog2_idx: floor(log2(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG2_IDX_H
#define OEISC_UTILS_ILOG2_IDX_H
#include <stdint.h>
#include <stdlib.h>

/* compute floor(log2(x)) for x > 0  */
static idx_t ilog2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

#endif
