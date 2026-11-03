/* gcd_i128: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_I128_H
#define OEISC_UTILS_GCD_I128_H
#include <stdint.h>
#include <stdlib.h>

/* 128‑bit GCD used only when seq_t is at most 64 bits. */
static inline __int128 gcd_i128(__int128 a, __int128 b)
{
    while (b != 0) {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a > 0 ? a : -a;
}

#endif
