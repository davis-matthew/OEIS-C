/* smallest_prime_factor: the smallest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SMALLEST_PRIME_FACTOR_H
#define OEISC_UTILS_SMALLEST_PRIME_FACTOR_H
#include <stddef.h>
#include <stdint.h>

/* Smallest prime factor of a 64‑bit integer (returns the number itself if prime). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n < 2) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n;                         /* n is prime */
}

#endif
