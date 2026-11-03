/* is_square_idx: whether n is a perfect square.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUARE_IDX_H
#define OEISC_UTILS_IS_SQUARE_IDX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Small integer square test (uses a binary search on the 128‑bit product)
 * to avoid overflow when squaring 64‑bit indices. */
static inline bool is_square_idx(idx_t n) {
    if (n < 2) return true;          /* 0 and 1 are squares */
    idx_t low = 1, high = n / 2 + 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return true;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}

#endif
