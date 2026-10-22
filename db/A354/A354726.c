/*
 * A354726  a(1)=2. Thereafter a(n) is the least prime q>a(n-1) such that
 *          Omega(a(n-1)+a(n))=3.
 * https://oeis.org/A354726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   count_prime_factors, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A354726: a(1)=2; for n>1 a(n) is the least prime q>a(n-1)
   such that the sum a(n-1)+q has exactly three prime factors
   counted with multiplicity. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, return 0 */

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Grow cache to hold at least n terms */
    if (cache_len < (size_t)n) {
        size_t needed = (size_t)n;
        if (cache_cap == 0) cache_cap = 1;
        while (cache_cap < needed) cache_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;

        /* Initialise first term if cache was empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)2;
            cache_len = 1;
        }

        /* Generate missing terms */
        seq_t prev = cache[cache_len - 1];
        for (size_t i = cache_len; i < needed; ++i) {
            /* start search from the next prime after prev */
            uint64_t start = (uint64_t)prev + 1;
            uint64_t q = next_prime(start);
            while (1) {
                seq_t sum = prev + (seq_t)q;
                uint64_t sum_u = (uint64_t)sum; /* assume sum fits in 64-bit */
                if (count_prime_factors(sum_u) == 3) break;
                q = next_prime(q + 1);
            }
            cache[i] = (seq_t)q;
            prev   = cache[i];
        }
        cache_len = needed;
    }

    return cache[n - 1];
}
