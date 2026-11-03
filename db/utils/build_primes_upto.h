/* build_primes_upto: the primes up to a limit.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BUILD_PRIMES_UPTO_H
#define OEISC_UTILS_BUILD_PRIMES_UPTO_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* The build supplies seq_t and idx_t; we rely on their definitions. */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    /* Simple Eratosthenes sieve.  limit >= 2. */
    size_t i, j;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve) return;             /* out of memory; return without primes */
    for (i = 2; i <= limit; ++i) sieve[i] = true;
    for (i = 2; i * i <= limit; ++i) if (sieve[i]) {
        for (j = i * i; j <= limit; j += i) sieve[j] = false;
    }

    /* Count primes. */
    *cnt_out = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) ++(*cnt_out);

    *primes_out = (uint64_t *)malloc(*cnt_out * sizeof(uint64_t));
    if (!*primes_out) {
        free(sieve);
        *cnt_out = 0;
        return;
    }
    size_t pos = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) (*primes_out)[pos++] = (uint64_t)i;

    free(sieve);
}

#endif
