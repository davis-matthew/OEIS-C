/* popcount_seq_to_i32: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_SEQ_TO_I32_H
#define OEISC_UTILS_POPCOUNT_SEQ_TO_I32_H
#include <stdint.h>

/* Count the number of 1‑bits in a seq_t value. */
static int popcount_seq_to_i32(seq_t x)
{
    int cnt = 0;
    while (x) {
        cnt += (int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

#endif
