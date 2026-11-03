/* gcd_i32: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_I32_H
#define OEISC_UTILS_GCD_I32_H
#include <stddef.h>
#include <stdint.h>

static int gcd_i32(int a, int b) {
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

#endif
