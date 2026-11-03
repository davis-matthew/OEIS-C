/* ctz_i64: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_I64_H
#define OEISC_UTILS_CTZ_I64_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int ctz_i64(long long x) {
    int r = 0;
    while (!(x & 1)) { x >>= 1; r++; }
    return r;
}

#endif
