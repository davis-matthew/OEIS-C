/* prime_pi: the number of primes <= n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_PRIME_PI_H
#define OEISC_UTILS_PRIME_PI_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the prime counting function pi(n) for any n ≥ 0
 * by sieving up to n on each call. */
static seq_t prime_pi(idx_t n)
{
    if (n < 2)
        return 0;

    /* Allocate a boolean array for the sieve */
    unsigned char *is_prime = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!is_prime)
        abort();                     /* allocation failed */

    /* Initialise all entries as prime except 0 and 1 */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= n; q += p)
                is_prime[q] = 0;
        }
    }

    /* Count primes up to n */
    seq_t count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++count;

    free(is_prime);
    return count;
}

#endif
