/* num_digits_u64: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_U64_H
#define OEISC_UTILS_NUM_DIGITS_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static inline int num_digits_u64(uint64_t x) {
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

#endif
