/* ipow_u64_u32_to_u64: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_U32_TO_U64_H
#define OEISC_UTILS_IPOW_U64_U32_TO_U64_H
#include <stdbool.h>
#include <stdint.h>

static inline uint64_t ipow_u64_u32_to_u64(uint64_t base, unsigned exp)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

#endif
