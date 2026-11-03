/* num_digits_seq_to_u32: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_SEQ_TO_U32_H
#define OEISC_UTILS_NUM_DIGITS_SEQ_TO_U32_H
#include <stdbool.h>
#include <stdint.h>

/* Return the number of decimal digits of x.  Treat 0 as having one digit. */
static unsigned int num_digits_seq_to_u32(seq_t x)
{
    unsigned int d = 0;
    while (x > 0)
    {
        x /= 10;
        d++;
    }
    return d == 0 ? 1 : d;
}

#endif
