/* is_square_u64: whether n is a perfect square.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUARE_U64_H
#define OEISC_UTILS_IS_SQUARE_U64_H
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool is_square_u64(uint64_t v)
{
    uint64_t s = (uint64_t)sqrt((long double)v);
    return s * s == v;
}

#endif
