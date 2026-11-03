/* gcd_i64: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_I64_H
#define OEISC_UTILS_GCD_I64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static long long gcd_i64(long long x, long long y) {
    while (y) { long long t = x % y; x = y; y = t; }
    return x;
}

#endif
