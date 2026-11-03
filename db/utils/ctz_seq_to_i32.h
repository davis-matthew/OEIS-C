/* ctz_seq_to_i32: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_SEQ_TO_I32_H
#define OEISC_UTILS_CTZ_SEQ_TO_I32_H
#include <stdbool.h>
#include <stdint.h>

/* Compute the exponent of 2 in x.  Works for any unsigned integer type
   including uint8_t, uint16_t, uint32_t, uint64_t, and unsigned __int128. */
static inline int ctz_seq_to_i32(seq_t x)
{
    int e = 0;
    seq_t y = x;
    while ((y & 1) == 0) {
        y >>= 1;
        ++e;
    }
    return e;
}

#endif
