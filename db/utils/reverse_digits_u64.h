/* reverse_digits_u64: n with its decimal digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_DIGITS_U64_H
#define OEISC_UTILS_REVERSE_DIGITS_U64_H
#include <stdint.h>
#include <stdlib.h>

/* Helper: reverse the decimal digits of a 64‑bit integer. */
static uint64_t reverse_digits_u64(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

#endif
