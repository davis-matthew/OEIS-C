/* prime_pi_idx: the number of primes <= n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_PRIME_PI_IDX_H
#define OEISC_UTILS_PRIME_PI_IDX_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Return π(n), the number of primes ≤ n, using a simple sieve. */
static uint64_t prime_pi_idx(idx_t n)
{
    if (n < 2) return 0;

    size_t sz = (size_t)n + 1;
    char *is_prime = (char*)malloc(sz);
    if (!is_prime) return 0;                      /* safety, unlikely to happen */

    memset(is_prime, 1, sz);
    is_prime[0] = 0;
    is_prime[1] = 0;

    size_t limit = (size_t)n;
    for (size_t p = 2; p * p <= limit; ++p)
    {
        if (!is_prime[p]) continue;
        for (size_t q = p * p; q <= limit; q += p)
            is_prime[q] = 0;
    }

    uint64_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    free(is_prime);
    return count;
}

#endif
