/* num_digits_u32_to_i32: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_U32_TO_I32_H
#define OEISC_UTILS_NUM_DIGITS_U32_TO_I32_H
#include <stddef.h>
#include <stdint.h>

/* small helper: number of decimal digits of a 32‑bit unsigned integer   */
static inline int num_digits_u32_to_i32(uint32_t x)
{
    if (x >= 1000000000u) return 10;
    if (x >= 100000000u)  return 9;
    if (x >= 10000000u)   return 8;
    if (x >= 1000000u)    return 7;
    if (x >= 100000u)     return 6;
    if (x >= 10000u)      return 5;
    if (x >= 1000u)       return 4;
    if (x >= 100u)        return 3;
    if (x >= 10u)         return 2;
    return 1;
}

#endif
