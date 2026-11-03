/* ipow_idx_to_idx: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_IDX_TO_IDX_H
#define OEISC_UTILS_IPOW_IDX_TO_IDX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Compute base^exp for unsigned base and idx_t exponent. */
static idx_t ipow_idx_to_idx(idx_t base, idx_t exp)
{
    idx_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

#endif
