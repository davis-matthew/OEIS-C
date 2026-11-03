/* reverse_digits_idx: n with its decimal digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_DIGITS_IDX_H
#define OEISC_UTILS_REVERSE_DIGITS_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* return decimal digit reversal of a non‑negative integer */
static inline idx_t reverse_digits_idx(idx_t x)
{
    idx_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

#endif
