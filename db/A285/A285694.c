/*
 * A285694  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of 10 primes.
 * https://oeis.org/A285694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   count_prime_factors, is_prime_big, next_prime, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(1) = 2;  a(n+1) = smallest prime > a(n) such that
   a(n+1) - a(n) has exactly 10 prime factors (counted with multiplicity). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/count_prime_factors.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Grow cache to at least 'needed' elements. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 4 : *cap * 2;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp;
        if (*ptr == NULL) {
            tmp = seq_alloc(newcap);
        } else {
            tmp = seq_realloc(*ptr, *cap, newcap);
        }
        *ptr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache of previously generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;   /* number of terms stored, cache[0] = a(1) */

    /* Initialise cache with first term. */
    if (cache_size == 0) {
        ensure_capacity(1, &cache_cap, &cache);
        cache[0] = (seq_t)2;     /* a(1) = 2 */
        cache_size = 1;
    }

    /* Generate terms up to requested index. */
    while (cache_size < n) {
        seq_t last = cache[cache_size - 1];

        /* Find the next prime > last. */
        uint64_t start = (uint64_t)last + 1;
        uint64_t p = next_prime(start);
        seq_t candidate = (seq_t)p;

        /* Loop until we find a prime whose distance from last has 10
           prime factors (with multiplicity). */
        while (true) {
            seq_t diff = candidate - last;
            if (count_prime_factors(diff) == (seq_t)10) {
                break;
            }
            /* Advance to the next prime. */
            uint64_t np = (uint64_t)candidate + 1;
            p = next_prime(np);
            candidate = (seq_t)p;
        }

        /* Append candidate to cache. */
        ensure_capacity(cache_size + 1, &cache_cap, &cache);
        cache[cache_size] = candidate;
        ++cache_size;
    }

    return cache[n - 1];
}
