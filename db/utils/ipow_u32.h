/* ipow_u32: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U32_H
#define OEISC_UTILS_IPOW_U32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: fast exponentiation on seq_t                                 */
static inline seq_t ipow_u32(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

#endif
