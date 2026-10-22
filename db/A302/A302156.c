/*
 * A302156  a(n) = Product_{k=1..n} prime(k+1)^(n-k+1).
 * https://oeis.org/A302156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} prime(k+1)^(n-k+1) a(0) = 1 Recurrence:
 *            a(n) = a(n-1) * (primorial(n+1) / 2) where primorial(m) =
 *            product of the first m primes
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Static cache for computed values, capacity, and helper data. */
    static seq_t *cache          = NULL;
    static size_t cache_cap      = 0;
    static idx_t last_cached     = 0;
    static seq_t primorial       = 0; /* product of first (last_cached+1) primes */

    /* One-time initialization. */
    if (cache == NULL) {
        /* Ensure space for the first element. */
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        last_cached = 0;
        /* primorial(1) = product of the first 1 prime = 2. */
        primorial = (seq_t)get_prime(1);
    }

    /* Extend cache up to requested index, if necessary. */
    if (n > last_cached) {
        /* Ensure the array holds entries up to n. */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t i = last_cached + 1; i <= n; ++i) {
            /* Update primorial to product of first (i+1) primes. */
            seq_t prime_next = (seq_t)get_prime(i + 1);
            primorial *= prime_next;

            /* a(i) = a(i-1) * primorial / 2. */
            seq_t prev = cache[i - 1];
            cache[i] = (prev * primorial) / (seq_t)2;
        }
        last_cached = n;
    }

    return cache[n];
}
