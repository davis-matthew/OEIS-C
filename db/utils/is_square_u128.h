/* is_square_u128: whether n is a perfect square.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUARE_U128_H
#define OEISC_UTILS_IS_SQUARE_U128_H
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline bool is_square_u128(unsigned __int128 x)
{
    unsigned long long r = sqrtl((long double)x);
    unsigned __int128 r2 = (unsigned __int128)r * r;
    if (r2 == x) return true;
    r2 = (unsigned __int128)(r + 1) * (r + 1);
    if (r2 == x) return true;
    r2 = (unsigned __int128)(r - 1) * (r - 1);
    return r2 == x;
}

#endif
