/* gcd_u16: the greatest common divisor of a and b.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GCD_U16_H
#define OEISC_UTILS_GCD_U16_H
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- integer helpers ----------------------------------------------------*/
static inline uint16_t gcd_u16(uint16_t a, uint16_t b)
{
    while (b) { uint16_t t = a % b; a = b; b = t; }
    return a;
}

#endif
