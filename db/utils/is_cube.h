/* is_cube: whether n is a perfect cube.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_CUBE_H
#define OEISC_UTILS_IS_CUBE_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------
   Check whether a 64-bit unsigned integer is a perfect cube.
   ----------
   Returns true if x is a perfect cube. */
static bool is_cube(uint64_t x)
{
    /* quick bounds check */
    if (x == 0) return true;   /* 0^3 == 0, but phi(0) is not used */
    /* approximate cube root using 64-bit arithmetic */
    uint64_t lo = 0, hi = 1ULL << 21;  /* 2^21 > 2^63^(1/3) ~ 2^21.3 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t cube = (__uint128_t)mid * mid * mid;
        if (cube <= x) lo = mid;
        else hi = mid - 1;
    }
    return ((uint64_t)lo * lo * lo) == x;
}

#endif
