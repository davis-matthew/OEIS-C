/* modinv_i64: the inverse of a modulo m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MODINV_I64_H
#define OEISC_UTILS_MODINV_I64_H
#include <stdint.h>
#include <stdlib.h>

/*---  Small integer modular inverse ---------------------------------------*/
static int64_t modinv_i64(int64_t a, int64_t mod)
{
    int64_t b = mod, u = 1, v = 0;
    while (b) {
        int64_t t = a / b;
        int64_t tmp = a - t * b;   a = b;   b = tmp;
        tmp = u - t * v;            u = v;   v = tmp;
    }
    if (u < 0) u += mod;
    return u;
}

#endif
