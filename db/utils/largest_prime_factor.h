/* largest_prime_factor: the largest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_LARGEST_PRIME_FACTOR_H
#define OEISC_UTILS_LARGEST_PRIME_FACTOR_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Greatest‑prime‑factor routine (works for any uint64_t). */
static uint64_t largest_prime_factor(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t maxp = 1;
    while ((n & 1ULL) == 0) { maxp = 2; n >>= 1; }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        while (n % d == 0) { maxp = d; n /= d; }
    }
    if (n > 1) maxp = n;
    return maxp;
}

#endif
