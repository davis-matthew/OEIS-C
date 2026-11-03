/* reverse_digits_idx_to_seq: n with its decimal digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_DIGITS_IDX_TO_SEQ_H
#define OEISC_UTILS_REVERSE_DIGITS_IDX_TO_SEQ_H
#include <stdint.h>

/* Reverse the decimal representation of an idx_t value.
   The result is stored in seq_t (which may be larger than idx_t). */
static inline seq_t reverse_digits_idx_to_seq(idx_t x)
{
    seq_t rev = 0;
    while (x != 0) {
        rev = rev * 10 + (seq_t)(x % 10);
        x /= 10;
    }
    return rev;
}

#endif
