/* modinv_idx: the inverse of a modulo m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MODINV_IDX_H
#define OEISC_UTILS_MODINV_IDX_H
#include <stdint.h>

/* Compute modular inverse of a modulo mod.
   Returns 0 if a and mod are not coprime. */
static inline idx_t modinv_idx(idx_t a, idx_t mod)
{
    idx_t a0 = a, b0 = mod;
    int64_t x0 = 1, y0 = 0;
    int64_t x1 = 0, y1 = 1;
    while (b0 != 0) {
        idx_t q = a0 / b0;
        idx_t r = a0 % b0;
        int64_t x2 = x0 - (int64_t)q * x1;
        int64_t y2 = y0 - (int64_t)q * y1;
        a0 = b0; b0 = r;
        x0 = x1; x1 = x2;
        y0 = y1; y1 = y2;
    }
    if (a0 != 1) return 0;                /* not coprime */
    int64_t inv = x0;
    inv %= (int64_t)mod;
    if (inv < 0) inv += (int64_t)mod;
    return (idx_t)inv;                     /* 1 <= inv < mod */
}

#endif
