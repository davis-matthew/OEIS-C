/* num_divisors: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_H
#define OEISC_UTILS_NUM_DIVISORS_H
#include <stdint.h>

/* Compute the divisor count of a 64‑bit integer using trial division. */
static uint64_t num_divisors(uint64_t n)
{
    if (n <= 1)
        return 1; /* d(1) = 1 */

    uint64_t count = 1;
    /* Handle factor 2 separately for a tiny speedup */
    if ((n & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++exp;
        }
        count *= (exp + 1);
    }

    /* Check odd divisors up to sqrt(n) */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }

    /* If anything remains, it is a prime factor > sqrt(original n). */
    if (n > 1)
        count *= 2;

    return count;
}

#endif
