/* is_prime_u128: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_U128_H
#define OEISC_UTILS_IS_PRIME_U128_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Helper: primality test for unsigned __int128 values ---------- */
static bool is_prime_u128(__uint128_t m)
{
    if (m < 2) return false;
    if ((m & 1) == 0) return m == 2;
    for (uint64_t d = 3; (__uint128_t)d * d <= m; d += 2) {
        if (m % d == 0) return false;
    }
    return true;
}

#endif
