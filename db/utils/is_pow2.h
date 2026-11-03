/* is_pow2: whether n is a power of 2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_POW2_H
#define OEISC_UTILS_IS_POW2_H
#include <stdbool.h>
#include <stdint.h>

static bool is_pow2(seq_t x)
{
    return x && ((x & (x - 1)) == 0);
}

#endif
