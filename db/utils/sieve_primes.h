/* sieve_primes: the primes up to a limit.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIEVE_PRIMES_H
#define OEISC_UTILS_SIEVE_PRIMES_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper: generates all primes <= limit using simple Sieve of Eratosthenes. */
static void sieve_primes(idx_t limit, idx_t **primes_out, size_t *count_out) {
    /* Allocate boolean array for sieve */
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    /* Count primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;
    /* Allocate primes array */
    idx_t *primes = (idx_t*)malloc(count * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;
    free(is_prime);
    *primes_out = primes;
    *count_out = count;
}

#endif
