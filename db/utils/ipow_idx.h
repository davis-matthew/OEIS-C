/* ipow_idx: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_IDX_H
#define OEISC_UTILS_IPOW_IDX_H
#include <stdint.h>
#include <stdlib.h>

/* fast exponentiation by squaring: base^exp in seq_t */
static seq_t ipow_idx(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

#endif
