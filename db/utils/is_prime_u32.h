/* is_prime_u32: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_U32_H
#define OEISC_UTILS_IS_PRIME_U32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------- Helper functions --------------------------------------- */
static bool is_prime_u32(unsigned int x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

#endif
