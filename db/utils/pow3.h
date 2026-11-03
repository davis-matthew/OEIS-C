/* pow3: 3^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW3_H
#define OEISC_UTILS_POW3_H
#include <stdint.h>

static seq_t pow3(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

#endif
