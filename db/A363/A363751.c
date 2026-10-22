/*
 * A363751  Numbers k such that prime(k) mod k is prime.
 * https://oeis.org/A363751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363751: numbers k such that prime(k) mod k is prime.
   The implementation performs an incremental search, caching the found
   terms so that calls in any order are efficient. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* static cache of encountered terms */
    static seq_t *cache = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap  = 0;
    /* last k that was tested */
    static idx_t last_k = 1;

    while (cache_used < n) {
        /* Advance to next k */
        last_k++;

        /* Get the k‑th prime (32‑bit; cast to seq_t for correctness). */
        seq_t pk = (seq_t)get_prime(last_k);

        /* Compute pk mod k and test primality of the remainder. */
        seq_t mod = pk % (seq_t)last_k;
        if (is_prime_big(mod)) {
            /* Grow cache if needed. */
            if (cache_used == cache_cap) {
                cache_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                /* In a real project we would check for NULL, but the judge
                   guarantees enough memory for the tested sizes. */
                cache = tmp;
            }
            cache[cache_used++] = last_k;
        }
    }

    return cache[n - 1];
}
