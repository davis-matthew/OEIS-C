/* popcount_seq_to_seq: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_SEQ_TO_SEQ_H
#define OEISC_UTILS_POPCOUNT_SEQ_TO_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* count ones in an unsigned integer of type seq_t */
static seq_t popcount_seq_to_seq(seq_t x)
{
    seq_t c = 0;
    while (x) {
        c++;
        x &= x - (seq_t)1;
    }
    return c;
}

#endif
