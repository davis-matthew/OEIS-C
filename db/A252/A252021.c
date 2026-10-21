/*
 * A252021  Primes p such that (p - 1)/tau(p - 1) is also prime.
 * https://oeis.org/A252021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 335, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th prime p such that (p‑1)/τ(p‑1) is also prime
   where τ(m) = number of positive divisors of m. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_prime.h"

/* Static cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 1; /* last prime examined */

/* Grow the cache array when needed. */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A252021.  For n==0 a(n) is undefined (returns 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1 */

    while (cache_len < (size_t)n) {
        /* Find the next prime p > last_checked. */
        uint64_t p;
        /* Use a simple incremental search; the sequence is small enough
           that this is fast for n up to ~10000. */
        p = last_checked + 1;
        while (!is_prime((idx_t)p))
            ++p;
        last_checked = p;

        /* Compute (p-1)/τ(p-1) and test primality of the quotient. */
        uint64_t m = p - 1;
        uint64_t tau = divisor_count_u64(m);
        if (m % tau != 0)
            continue;  /* not divisible, skip */

        uint64_t q = m / tau;
        if (!is_prime((idx_t)q))
            continue;  /* quotient not prime */

        /* Store the found prime in the cache. */
        ensure_cache_capacity();
        cache[cache_len++] = (seq_t)p;
    }

    return cache[n - 1];
}
