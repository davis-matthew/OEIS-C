/* is_square_u32: whether n is a perfect square.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUARE_U32_H
#define OEISC_UTILS_IS_SQUARE_U32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* helper: test whether a small unsigned integer is a perfect square */
static bool is_square_u32(unsigned v)
{
    if (v <= 1) return true;
    unsigned r = 1;
    while (r * r < v) ++r;
    return r * r == v;
}

#endif
