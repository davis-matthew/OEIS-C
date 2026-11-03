/* is_prime: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_H
#define OEISC_UTILS_IS_PRIME_H
#include <stdbool.h>
#include <stdint.h>

/* Tiny primality test: works for the range needed (n <= about 10000) */
static bool is_prime(idx_t x)
{
    if (x < 2)                return false;
    if (x % 2ULL == 0ULL)     return x == 2;
    for (idx_t d = 3; d*d <= x; d += 2)
        if (x % d == 0)       return false;
    return true;
}

#endif
