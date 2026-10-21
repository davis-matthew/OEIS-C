/*
 * A284290  Primes containing a digit 4.
 * https://oeis.org/A284290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 2441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th prime containing the digit 4 (OEIS A284290). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t next_candidate = 2;

    /* Undefined for n == 0. */
    if (n == 0)
        return 0;

    /* Initial cache allocation. */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;
    }

    /* Return a cached value if we have already generated it. */
    if (cache_size >= n)
        return cache[n - 1];

    /* Generate prime candidates until we have at least n terms. */
    while (cache_size < n) {
        /* Find the next prime that contains a digit 4. */
        for (;;) {
            if (is_prime(next_candidate)) {
                idx_t t = next_candidate;
                while (t > 0) {
                    if (t % 10 == 4)
                        break;                 /* found a 4 */
                    t /= 10;
                }
                if (t == 0)                    /* no 4 found; continue */
                    ;
                else
                    break;                     /* prime with 4 found */
            }
            ++next_candidate;
        }

        /* Grow the cache if necessary. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                cache_size = 0;
                cache_cap   = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the newly found term. */
        cache[cache_size] = (seq_t)next_candidate;
        ++cache_size;
        ++next_candidate;
    }

    return cache[n - 1];
}
