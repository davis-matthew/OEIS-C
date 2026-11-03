/* gcd_idx_to_idx: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_IDX_TO_IDX_H
#define OEISC_UTILS_GCD_IDX_TO_IDX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Euclidean gcd for a seq_t value and an idx_t divisor (result fits in idx_t) */
static inline idx_t gcd_idx_to_idx(seq_t a, idx_t b)
{
    while (b != 0) {
        seq_t r = a % (seq_t)b;
        a = (seq_t)b;
        b = (idx_t)r;
    }
    return (idx_t)a;
}

#endif
