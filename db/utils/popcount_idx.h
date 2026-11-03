/* popcount_idx: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_IDX_H
#define OEISC_UTILS_POPCOUNT_IDX_H
#include <stdint.h>

static inline idx_t popcount_idx(idx_t x) {
    return __builtin_popcountll(x);
}

#endif
