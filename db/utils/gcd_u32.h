/* gcd_u32: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_U32_H
#define OEISC_UTILS_GCD_U32_H
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* small gcd --------------------------------------------------------------- */
static uint32_t gcd_u32(uint32_t a, uint32_t b) {
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

#endif
