/* num_digits0_seq_to_i32: the number of decimal digits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS0_SEQ_TO_I32_H
#define OEISC_UTILS_NUM_DIGITS0_SEQ_TO_I32_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Count decimal digits of a non‑zero seq_t value. */
static inline int num_digits0_seq_to_i32(seq_t v)
{
    int d = 0;
    while (v != 0) {
        v /= 10;
        d++;
    }
    return d;
}

#endif
