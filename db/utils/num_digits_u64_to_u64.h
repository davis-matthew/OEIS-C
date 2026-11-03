/* num_digits_u64_to_u64: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_U64_TO_U64_H
#define OEISC_UTILS_NUM_DIGITS_U64_TO_U64_H
#include <stdbool.h>
#include <stdint.h>

/* number of decimal digits of a non‑zero unsigned integer */
static inline uint64_t num_digits_u64_to_u64(uint64_t x)
{
    uint64_t d = 0;
    do {
        x /= 10;
        ++d;
    } while (x > 0);
    return d;
}

#endif
