/* is_pow2_idx: whether n is a power of 2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_POW2_IDX_H
#define OEISC_UTILS_IS_POW2_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* Check whether n is a power of two (n > 0). */
static inline bool is_pow2_idx(idx_t n)
{
    return n != 0 && (n & (n - 1)) == 0;
}

#endif
