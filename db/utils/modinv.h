/* modinv: the inverse of a modulo m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MODINV_H
#define OEISC_UTILS_MODINV_H
#include <stdbool.h>
#include <stdint.h>

/* extended Euclid to find inverse of a modulo m (assumes gcd(a,m)=1) */
static uint64_t modinv(uint64_t a, uint64_t m)
{
    int64_t b = m, u = 1, v = 0;
    while (b) {
        uint64_t q = a / b;
        uint64_t r = a % b;
        int64_t m2 = u - (int64_t)q * v;
        a = b; b = r; u = v; v = m2;
    }
    if (u < 0) u += m;
    return (uint64_t)u;
}

#endif
