/* num_digits_idx_to_u32: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_IDX_TO_U32_H
#define OEISC_UTILS_NUM_DIGITS_IDX_TO_U32_H
#include <stdint.h>

/* Count the decimal digits of an idx_t value (at least 1 digit). */
static unsigned num_digits_idx_to_u32(idx_t x)
{
    unsigned d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

#endif
