/* simple_sieve: the primes up to a limit.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIMPLE_SIEVE_H
#define OEISC_UTILS_SIMPLE_SIEVE_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- sieve of Eratosthenes up to LIMIT.
 *   primes_out will point to an array containing all primes ≤ LIMIT
 *   (allocated with malloc); count_out receives the number of primes.
 *   The caller must free the array after use. ---------------- */
static void simple_sieve(uint32_t limit, uint32_t **primes_out,
                         size_t *count_out)
{
    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    if (!comp) { *primes_out = NULL; *count_out = 0; return; }
    size_t capacity = limit / 10 + 10;
    uint32_t *primes = (uint32_t *)malloc(capacity * sizeof(uint32_t));
    size_t count = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            if (count == capacity) {
                capacity <<= 1;
                primes = (uint32_t *)realloc(primes,
                                             capacity * sizeof(uint32_t));
            }
            primes[count++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }
    free(comp);
    *primes_out = primes;
    *count_out = count;
}

#endif
