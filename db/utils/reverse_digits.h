/* reverse_digits: n with its decimal digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_DIGITS_H
#define OEISC_UTILS_REVERSE_DIGITS_H
#include <stdint.h>
#include <stdlib.h>

/* Reverse the decimal representation of a non‑negative integer stored in seq_t. */
static seq_t reverse_digits(seq_t v)
{
    seq_t rev = 0;
    seq_t t   = v;
    while (t > 0) {
        rev = rev * 10 + (t % 10);
        t   = t / 10;
    }
    return rev;
}

#endif
