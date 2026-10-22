/*
 * A320703  Indices of primes followed by a gap (distance to next larger
 *          prime) of 10.
 * https://oeis.org/A320703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 5976, u32 n <= 8870, u64 n <= 8870,
 *            u128 n <= 8870, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A320703: indices of primes followed by a gap of 10.
   A(n) is the index i such that prime(i) and prime(i+1) = prime(i) + 10.
   Computed by incremental prime searching. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found indices */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static state for prime generation */
    static uint64_t last_prime = 0;   /* last prime processed */
    static idx_t   last_index  = 0;   /* 1‑based index of last_prime */
    static bool    initialized = false;

    if (!initialized) {
        /* Initialisation: first prime is 2 at index 1 */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
        last_prime = 2;
        last_index = 1;
        initialized = true;
    }

    /* Generate primes until we have at least n indices in the cache */
    while (cache_len < n) {
        /* Find next prime after last_prime */
        uint64_t cand = last_prime + 1;
        while (!is_prime(cand)) {
            ++cand;
        }
        idx_t cur_index = last_index + 1;

        /* If the gap between consecutive primes is 10, store the index of the
           first prime in the gap (i.e., last_index). */
        if (cand - last_prime == 10) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_index;
        }

        /* Advance to the next prime */
        last_prime = cand;
        last_index = cur_index;
    }

    return cache[n - 1];
}
