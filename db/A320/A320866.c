/*
 * A320866  Primes such that p + digitsum(p, base 4) is again a prime.
 * https://oeis.org/A320866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function searches for primes p such that p + digit_sum(p,
 *            base 4) is also prime, returning the n‑th such prime (with
 *            offset 1)
 * Range:     u8 n <= 14, u16 n <= 957, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */

/* Compute the n-th term of A320866. */
seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored so far */
    static size_t cache_cap  = 0;   /* capacity of the cache */
    static idx_t next_candidate = 2;/* next integer to test for primality */

    if (n == 0)            /* undefined for offset 1, but guard */
        return 0;

    /* Expand the cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        idx_t p = next_candidate;

        /* Compute digit sum of p in base 4. */
        idx_t tmp  = p;
        idx_t sum  = 0;
        while (tmp) {
            sum += tmp % 4;
            tmp /= 4;
        }

        /* Check whether p + sum is also prime. */
        idx_t q = p + sum;
        if (is_prime(q)) {
            /* Store p in the cache if it fits into seq_t. */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)p;
        }

        /* Advance to the next candidate integer. */
        ++next_candidate;
    }

    return cache[n - 1];
}
