/* valuation: the exponent of p in n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_VALUATION_H
#define OEISC_UTILS_VALUATION_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t valuation(uint64_t c, uint64_t p) {
    uint64_t e = 0;
    while (c > 0 && c % p == 0) { c /= p; e++; }
    return e;
}

#endif
