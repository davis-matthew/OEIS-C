/* bit_length_u64_to_u32: the number of bits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIT_LENGTH_U64_TO_U32_H
#define OEISC_UTILS_BIT_LENGTH_U64_TO_U32_H
#include <stdbool.h>
#include <stdint.h>

/* length of the binary representation (no leading zeros) */
static inline unsigned bit_length_u64_to_u32(uint64_t v) {
    unsigned l = 0;
    while (v) { l++; v >>= 1; }
    return l;
}

#endif
