/* modinv_u32: the inverse of a modulo m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MODINV_U32_H
#define OEISC_UTILS_MODINV_U32_H
#include <stdint.h>
#include <stdlib.h>

static uint32_t modinv_u32(uint32_t a, uint32_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)a;
    while (newr != 0) {
        uint32_t q = (uint32_t)(r / newr);
        int64_t tmp = t - (int64_t)q * newt;
        t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr;
        r = newr; newr = tmp;
    }
    if (r > 1) return 0;          /* not invertible – should not happen for primes */
    if (t < 0) t += mod;
    return (uint32_t)t;
}

#endif
