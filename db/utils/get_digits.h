/* get_digits: the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_GET_DIGITS_H
#define OEISC_UTILS_GET_DIGITS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Convert a value of type seq_t to its decimal digits (least‑significant first).
   The resulting buffer contains the digits in reverse order and *len is set
   to the number of digits.  n==0 produces a single digit 0. */
static void get_digits(seq_t x, uint8_t *buf, size_t *len)
{
    size_t pos = 0;
    if (x == (seq_t)0) {
        buf[pos++] = 0;
    } else {
        while (x != (seq_t)0) {
            buf[pos++] = (uint8_t)(x % (seq_t)10);
            x /= (seq_t)10;
        }
    }
    *len = pos;
}

#endif
