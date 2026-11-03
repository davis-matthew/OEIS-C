/* is_prime_i32: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_I32_H
#define OEISC_UTILS_IS_PRIME_I32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool is_prime_i32(int x)
{
    if (x < 2) return false;
    for (int d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

#endif
