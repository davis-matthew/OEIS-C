/* num_digits0_seq: the number of decimal digits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS0_SEQ_H
#define OEISC_UTILS_NUM_DIGITS0_SEQ_H
#include <stdint.h>

/* Return the number of decimal digits of x. */
static idx_t num_digits0_seq(seq_t x)
{
    idx_t len = 0;
    seq_t t = x;
    while (t > 0) {
        t /= 10;
        len++;
    }
    return len;
}

#endif
