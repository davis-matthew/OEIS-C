/*
 * A244774  Prime numbers ending in the prime number 73.
 * https://oeis.org/A244774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime in the arithmetic progression 100*k+73
 * Range:     u8 n <= 2, u16 n <= 130, u32 n <= 131, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cached primes of the form 100*k+73. */
static seq_t *cache = NULL;   /* array of found primes */
static size_t cache_len = 0;  /* number of stored primes */
static size_t cache_cap = 0;  /* current capacity */

/* Append a prime to the cache, expanding it if necessary. */
static void append_prime(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(EXIT_FAILURE); /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th prime ≡ 73 (mod 100) for n ≥ 1. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* offset 0 is undefined, return 0 */

    /* Initialise cache with the first prime in the progression. */
    if (cache_len == 0) {
        append_prime((seq_t)73);
    }

    /* Extend the cache until it contains at least n primes. */
    while ((idx_t)cache_len < n) {
        seq_t last = cache[cache_len - 1];
        seq_t cand = last + (seq_t)100;   /* next candidate in the progression */

        /* Search forward until a prime is found. */
        while (!is_prime_big(cand)) {
            cand += (seq_t)100;
        }

        append_prime(cand);
    }

    return cache[n - 1];
}
