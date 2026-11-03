/* pow2_u64: 2^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW2_U64_H
#define OEISC_UTILS_POW2_U64_H
#include <stdint.h>
#include <stdlib.h>

static seq_t pow2_u64(uint64_t e) {
    seq_t r = 1, b = 2;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

#endif
