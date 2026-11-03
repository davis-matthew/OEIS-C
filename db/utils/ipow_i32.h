/* ipow_i32: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_I32_H
#define OEISC_UTILS_IPOW_I32_H
#include <stdint.h>

/* fast exponentiation for seq_t (sequential integer type) */
static seq_t ipow_i32(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

#endif
