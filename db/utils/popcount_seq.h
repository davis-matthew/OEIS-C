/* popcount_seq: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_SEQ_H
#define OEISC_UTILS_POPCOUNT_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* popcount for seq_t – works for any unsigned integer type or for a
   big‑integer class that supports &, >>, and zero comparison. */
static inline unsigned int popcount_seq(seq_t x)
{
    unsigned int c = 0;
    while (x)
    {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
}

#endif
