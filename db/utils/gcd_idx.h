/* gcd_idx: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_IDX_H
#define OEISC_UTILS_GCD_IDX_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Euclidean algorithm for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

#endif
