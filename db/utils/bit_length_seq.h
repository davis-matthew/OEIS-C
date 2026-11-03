/* bit_length_seq: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_SEQ_H
#define OEISC_UTILS_BIT_LENGTH_SEQ_H
#include <stdint.h>

/* bit length of a seq_t value */
static int bit_length_seq(seq_t v)
{
    int len = 0;
    while (v)
    {
        len++;
        v >>= 1;
    }
    return len;
}

#endif
