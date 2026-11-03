/* bigomega: the number of prime factors of n, with multiplicity.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIGOMEGA_H
#define OEISC_UTILS_BIGOMEGA_H
#include <stdint.h>
#include <stdlib.h>

static seq_t bigomega(idx_t n)
{
    /* Count prime factors of n with multiplicity. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2. */
    while ((n & 1) == 0) {
        n >>= 1;
        ++count;
    }

    /* Test odd divisors up to sqrt(n). */
    for (idx_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }

    /* If n > 1 now, it is a prime factor > sqrt(original n). */
    if (n > 1)
        ++count;

    return count;
}

#endif
