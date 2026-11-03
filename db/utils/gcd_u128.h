/* gcd_u128: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_U128_H
#define OEISC_UTILS_GCD_U128_H
#include <stdint.h>

/* Euclidean gcd for unsigned __int128 values */
static inline unsigned __int128 gcd_u128(unsigned __int128 a,
                                       unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

#endif
