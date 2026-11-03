/* num_digits: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_H
#define OEISC_UTILS_NUM_DIGITS_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static int num_digits(seq_t x)
{
    int len = 0;
    do {
        len++;
        x /= 10;
    } while (x != 0);
    return len;
}

#endif
