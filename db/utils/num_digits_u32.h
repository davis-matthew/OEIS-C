/* num_digits_u32: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_U32_H
#define OEISC_UTILS_NUM_DIGITS_U32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- integer utilities ----------------------------------------------------- */
static inline unsigned int num_digits_u32(uint32_t x)
{
    unsigned int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

#endif
