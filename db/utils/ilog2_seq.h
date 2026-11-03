/* ilog2_seq: floor(log2(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG2_SEQ_H
#define OEISC_UTILS_ILOG2_SEQ_H
#include <stdint.h>

/* Compute floor(log2(v)) for a generic seq_t value by repeated shifting.
   This works for all supported seq_t types (uint8_t … uint128_t, and big
   integer classes that support the >>= operator and <<). */
static inline unsigned ilog2_seq(seq_t v)
{
    unsigned r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

#endif
