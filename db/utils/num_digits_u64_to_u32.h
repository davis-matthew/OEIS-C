/* num_digits_u64_to_u32: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_U64_TO_U32_H
#define OEISC_UTILS_NUM_DIGITS_U64_TO_U32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper to compute number of decimal digits of a uint64_t. */
static unsigned num_digits_u64_to_u32(uint64_t n)
{
    unsigned d = 0;
    do { ++d; n /= 10; } while (n);
    return d;
}

#endif
