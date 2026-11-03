/* is_triangular_idx: whether n is a triangular number.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_TRIANGULAR_IDX_H
#define OEISC_UTILS_IS_TRIANGULAR_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* Check whether x is a triangular number.
   x is triangular iff 8*x+1 is a perfect square. */
static bool is_triangular_idx(idx_t x)
{
    idx_t t = 8 * x + 1;
    idx_t low = 0, high = t;           /* t fits in idx_t (uint64_t) */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == t) return true;
        if (sq < t)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}

#endif
