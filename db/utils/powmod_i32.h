/* powmod_i32: b^e mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POWMOD_I32_H
#define OEISC_UTILS_POWMOD_I32_H
#include <stddef.h>
#include <stdint.h>

static int powmod_i32(int base, int exp, int mod) {
    int r = 1;
    while (exp) {
        if (exp & 1) r = (int)((1LL * r * base) % mod);
        base = (int)((1LL * base * base) % mod);
        exp >>= 1;
    }
    return r;
}

#endif
