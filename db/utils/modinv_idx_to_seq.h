/* modinv_idx_to_seq: the inverse of a modulo m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MODINV_IDX_TO_SEQ_H
#define OEISC_UTILS_MODINV_IDX_TO_SEQ_H
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t modinv_idx_to_seq(idx_t a, idx_t m)
{
    if (m == 1) return 0;
    long long t = 0, newt = 1;
    unsigned long long r = m, newr = a % m;
    while (newr) {
        unsigned long long q = r / newr;
        unsigned long long tmp_r = r - q * newr;
        r = newr; newr = tmp_r;

        long long tmp_t = t;
        t = newt;
        newt = tmp_t - (long long)q * newt;
    }
    if (r != 1) return 0;           /* no inverse exists */
    if (t < 0) t += m;
    return (seq_t)t;
}

#endif
