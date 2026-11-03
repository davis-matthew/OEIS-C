/* ipow_u64_to_u64: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_TO_U64_H
#define OEISC_UTILS_IPOW_U64_TO_U64_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Compute base^exp for unsigned integers (exp ≥ 0). */
static uint64_t ipow_u64_to_u64(uint64_t base, uint64_t exp)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1ULL) result *= base;
        exp >>= 1ULL;
        if (exp) base *= base;
    }
    return result;
}

#endif
