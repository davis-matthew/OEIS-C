/*
 * A247874  Lesser of twin primes (p, q=p+2) such that 7 is a square mod p and
 *          mod q.
 * https://oeis.org/A247874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 251, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate over integers congruent to 1, 25, or 27 (mod 28).
   For each candidate p, if p and p+2 are prime then store p as a term. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoised terms */
    static size_t cache_size = 0;        /* number of stored terms */
    static size_t cache_cap  = 0;        /* allocated capacity */
    static idx_t next_i = 0;             /* next index i to process */

    if (n == 0) /* undefined for n = 0, but just return 0 */
        return (seq_t)0;

    /* grow the cache when necessary */
    while (cache_size < n) {
        /* candidate 1 : 1 + 28*i  (lower = 1 mod 28) */
        idx_t p1 = 1 + 28 * next_i;
        if (is_prime(p1) && is_prime(p1 + 2)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)p1;
        }

        /* candidate 2 : 25 + 28*i (lower = 25 mod 28) */
        idx_t p2 = 25 + 28 * next_i;
        if (is_prime(p2) && is_prime(p2 + 2)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)p2;
        }

        /* candidate 3 : 27 + 28*i (lower = 27 mod 28) */
        idx_t p3 = 27 + 28 * next_i;
        if (is_prime(p3) && is_prime(p3 + 2)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)p3;
        }

        ++next_i;
    }

    return cache[n - 1];
}
