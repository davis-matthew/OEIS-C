/* num_digits0: the number of decimal digits of n (0 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS0_H
#define OEISC_UTILS_NUM_DIGITS0_H
#include <math.h>
#include <stdint.h>

/* Compute the number of decimal digits of n (n >= 1). */
static inline uint64_t num_digits0(uint64_t n) {
    uint64_t cnt = 0;
    while (n) {
        ++cnt;
        n /= 10;
    }
    return cnt;
}

#endif
