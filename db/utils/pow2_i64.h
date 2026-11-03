/* pow2_i64: 2^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW2_I64_H
#define OEISC_UTILS_POW2_I64_H
#include <stdint.h>
#include <stdlib.h>

static seq_t pow2_i64(long long e)
{
    seq_t r = 1, b = 2;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

#endif
