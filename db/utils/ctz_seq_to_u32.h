/* ctz_seq_to_u32: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_SEQ_TO_U32_H
#define OEISC_UTILS_CTZ_SEQ_TO_U32_H
#include <stdbool.h>
#include <stdint.h>

static unsigned ctz_seq_to_u32(seq_t x)
{
    unsigned e = 0;
    while ((x & 1) == 0 && x != 0) {
        x >>= 1;
        ++e;
    }
    return e;
}

#endif
