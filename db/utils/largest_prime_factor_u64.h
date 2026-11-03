/* largest_prime_factor_u64: the largest prime factor of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_LARGEST_PRIME_FACTOR_U64_H
#define OEISC_UTILS_LARGEST_PRIME_FACTOR_U64_H
#include <stdbool.h>
#include <stdint.h>

/* Return the largest prime divisor of a 64‑bit integer x.
   If x ≤ 1, return 1 (no prime divisor). */
static seq_t largest_prime_factor_u64(uint64_t x)
{
    if (x <= 1)
        return 1;

    uint64_t largest = 1;

    /* factor 2 */
    if ((x & 1) == 0) {
        largest = 2;
        while ((x & 1) == 0)
            x >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            largest = p;
            while (x % p == 0)
                x /= p;
        }
    }

    /* remaining prime > √x */
    if (x > 1)
        largest = x;

    return (seq_t)largest;
}

#endif
