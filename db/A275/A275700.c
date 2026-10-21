/*
 * A275700  a(n) = Product_{d|n} prime(d).
 * https://oeis.org/A275700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} prime(d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_a_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_a_cache.h"

/* Prime cache: prime_cache[k-1] = k-th prime */
static const idx_t PRIME_BOUND = 200000;      /* covers n up to 10 000 (p_10000 = 104729) */
static seq_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Fill prime_cache with first primes up to PRIME_BOUND */
static void ensure_primes(void)
{
    if (prime_cache != NULL)
        return;

    size_t limit = PRIME_BOUND;
    size_t sz = limit + 1;

    char *is_prime = (char*)malloc(sz);
    if (!is_prime) exit(1);
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    prime_cache = (seq_t*)malloc(count * sizeof(seq_t));
    if (!prime_cache) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_cache[idx++] = (seq_t)i;

    prime_count = count;
    free(is_prime);
}

/* Return the k-th prime (k >= 1) */
static seq_t prime_of_idx(idx_t k)
{
    return prime_cache[k - 1];
}

/* Cached computed values of a(n) */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* just in case; offset starts at 1 */

    ensure_primes();
    ensure_a_cache((size_t)n, &a_cache, &a_cache_cap);

    if (a_cache[n - 1] != 0)
        return a_cache[n - 1];

    seq_t prod = (seq_t)1;

    /* compute integer sqrt of n */
    idx_t root = 1;
    while ((root + 1) * (root + 1) <= n)
        ++root;

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            prod *= prime_of_idx(d);
            idx_t e = n / d;
            if (e != d)
                prod *= prime_of_idx(e);
        }
    }

    a_cache[n - 1] = prod;
    return prod;
}
