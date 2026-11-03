/* ipow_u64_u32: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_U32_H
#define OEISC_UTILS_IPOW_U64_U32_H
#include <stdint.h>

/* Helper: integer power producing a value of type seq_t. */
static seq_t ipow_u64_u32(uint64_t base, unsigned exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp > 0) {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

#endif
