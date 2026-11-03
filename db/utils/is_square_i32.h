/* is_square_i32: whether n is a perfect square.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUARE_I32_H
#define OEISC_UTILS_IS_SQUARE_I32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether a small integer is a perfect square (works for values up to ~2000) */
static bool is_square_i32(int n)
{
    int r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r * r == n;
}

#endif
