/* ctz_idx: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_IDX_H
#define OEISC_UTILS_CTZ_IDX_H
#include <stdint.h>

static unsigned ctz_idx(idx_t x) {
    if (x == 0) return 64; /* for n = UINT64_MAX */
    unsigned c = 0;
    while ((x & 1ULL) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

#endif
