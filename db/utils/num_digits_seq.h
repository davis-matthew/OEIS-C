/* num_digits_seq: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_SEQ_H
#define OEISC_UTILS_NUM_DIGITS_SEQ_H
#include <stdint.h>
#include <stdlib.h>

/* Return the number of decimal digits of an arbitrary‑precision integer x. */
static idx_t num_digits_seq(seq_t x) {
    idx_t d = 1;
    /* Handle x == 0 (not expected for this sequence, but safe) */
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
        ++d;
    }
    return d;
}

#endif
