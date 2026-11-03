/* prime_nth: the n-th prime, prime_nth(1) = 2, from a table of primes that is
 * kept between calls and grows (the sieve limit doubles) as needed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_PRIME_NTH_H
#define OEISC_UTILS_PRIME_NTH_H
#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_nth_table = NULL;
static uint64_t prime_nth_count = 0, prime_nth_limit = 0;

static void prime_nth_sieve(uint64_t lim)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!comp) abort();
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (comp[i]) continue;
        ++cnt;
        if (i <= lim / i)
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    uint64_t *t = (uint64_t *)realloc(prime_nth_table, (size_t)(cnt ? cnt : 1) * sizeof *t);
    if (!t) abort();
    prime_nth_table = t;
    cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i)
        if (!comp[i]) prime_nth_table[cnt++] = i;
    free(comp);
    prime_nth_count = cnt;
    prime_nth_limit = lim;
}

static uint64_t prime_nth(uint64_t n)
{
    if (n == 0) return 0;
    while (prime_nth_count < n)
        prime_nth_sieve(prime_nth_limit ? 2 * prime_nth_limit : 1 << 16);
    return prime_nth_table[n - 1];
}

#endif
