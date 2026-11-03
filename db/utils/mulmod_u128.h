/* mulmod_u128: a*b mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULMOD_U128_H
#define OEISC_UTILS_MULMOD_U128_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
static unsigned __int128 mulmod_u128(unsigned __int128 a,
                                         unsigned __int128 b,
                                         unsigned __int128 mod)
{
    unsigned __int128 res = 0;
    while (b) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

#endif
