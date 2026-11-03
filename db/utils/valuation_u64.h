/* valuation_u64: the exponent of p in n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_VALUATION_U64_H
#define OEISC_UTILS_VALUATION_U64_H
#include <stdint.h>
#include <stdlib.h>

static long valuation_u64(uint64_t x, uint64_t p) {
    long e = 0;
    if (x == 0) return 0;
    while (x % p == 0) { x /= p; e++; }
    return e;
}

#endif
