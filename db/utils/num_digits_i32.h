/* num_digits_i32: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_I32_H
#define OEISC_UTILS_NUM_DIGITS_I32_H
#include <stddef.h>
#include <stdint.h>

/* small helper: number of decimal digits of an integer ≤ 2^31 */
static int num_digits_i32(int v)
{
    int len = 1;
    while (v >= 10) {
        ++len;
        v /= 10;
    }
    return len;
}

#endif
