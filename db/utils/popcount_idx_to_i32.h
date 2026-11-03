/* popcount_idx_to_i32: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_IDX_TO_I32_H
#define OEISC_UTILS_POPCOUNT_IDX_TO_I32_H
#include <stdint.h>

/* Return the number of 1‑bits in a 64‑bit unsigned integer.  */
static int popcount_idx_to_i32(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int cnt = 0;
    while (x) {
        x &= x - 1;
        ++cnt;
    }
    return cnt;
#endif
}

#endif
