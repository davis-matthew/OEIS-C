/* ipow_u64_idx: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_IDX_H
#define OEISC_UTILS_IPOW_U64_IDX_H
#include <stdint.h>

static inline seq_t ipow_u64_idx(uint64_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

#endif
