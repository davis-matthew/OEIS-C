/*
 * A338273  a(n) is the sum of A338270(n) and the average of the primes
 *          immediately before and after A338270(n).
 * https://oeis.org/A338273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A338270(n) + (prev_prime(A338270(n)) +
 *            next_prime(A338270(n)))/2 where A338270(n) is the nth prime p
 *            such that p + (prev + next)/2 is also prime. The function
 *            computes the nth term of the OEIS sequence A338273
 * Range:     u8 n <= 5, u16 n <= 371, u32 n <= 372, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* cache of computed a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* iterator over primes */
    static uint64_t current_prime = 0;   /* will be initialized on first use */
    static bool initialized = false;

    /* helper to grow the cache */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialisation on first call */
    if (!initialized) {
        current_prime = 3;   /* start with the smallest prime that has a previous */
        initialized = true;
    }

    /* generate terms until we have the nth one */
    while (cache_len < n) {
        /* current prime */
        uint64_t p = current_prime;

        /* get neighbouring primes */
        uint64_t prev = prev_prime(p);
        uint64_t next = next_prime(p);

        /* compute a(n) for this prime p */
        seq_t avg = ((seq_t)prev + (seq_t)next) / 2;
        seq_t a_val = (seq_t)p + avg;

        /* if the resulting number is prime, store it */
        if (is_prime_big(a_val)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = a_val;
        }

        /* advance to the next prime */
        current_prime = next_prime(p);
    }

    return cache[n - 1];
}
