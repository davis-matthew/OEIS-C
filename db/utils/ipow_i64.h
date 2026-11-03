/* ipow_i64: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_I64_H
#define OEISC_UTILS_IPOW_I64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t ipow_i64(seq_t b, long long e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

#endif
