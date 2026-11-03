/* isqrt_u128: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_U128_H
#define OEISC_UTILS_ISQRT_U128_H
#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
 * Returns floor(sqrt(x)). */
static inline unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = ((unsigned __int128)1) << 127;

    /* Reduce to the position of the highest set bit of x */
    while (bit > 0 && bit > x) bit >>= 1;

    for (; bit; bit >>= 1) {
        unsigned __int128 tmp = r | bit;
        /* Avoid overflow: check tmp*tmp <= x by division */
        if (tmp <= x / tmp)
            r = tmp;
    }
    return r;
}

#endif
