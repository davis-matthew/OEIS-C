/* num_digits_idx_to_seq: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_IDX_TO_SEQ_H
#define OEISC_UTILS_NUM_DIGITS_IDX_TO_SEQ_H
#include <stddef.h>
#include <stdint.h>

/* Return the number of decimal digits of an unsigned integer. */
static inline seq_t num_digits_idx_to_seq(idx_t x)
{
    seq_t d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

#endif
