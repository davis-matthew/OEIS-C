/* num_digits_u32_to_u64: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_U32_TO_U64_H
#define OEISC_UTILS_NUM_DIGITS_U32_TO_U64_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Count decimal digits of a non‑zero uint32_t. */
static size_t num_digits_u32_to_u64(uint32_t x)
{
    size_t d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

#endif
