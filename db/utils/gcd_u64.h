/* gcd_u64: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_U64_H
#define OEISC_UTILS_GCD_U64_H
#include <stdint.h>

static inline uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

#endif
