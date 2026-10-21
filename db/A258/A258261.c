/*
 * A258261  Primes p such that 3p - 4 is also prime.
 * https://oeis.org/A258261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the n-th prime p for which 3p-4 is also prime
   using a simple prime search and caching. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

typedef idx_t idx_t;  // provided externally
// seq_t is defined by the build system

static seq_t *cache = NULL;      // cached terms
static size_t cache_cap = 0;     // capacity of cache
static size_t cache_len = 0;     // number of terms stored
static uint64_t next_candidate = 2;  // next integer to test for primality

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  // out of memory
    cache = tmp;
    cache_cap = new_cap;
}

static void find_next_term(void)
{
    while (true) {
        /* Find next prime p */
        uint64_t p = next_candidate;
        while (!is_prime((idx_t)p)) p++;
        next_candidate = p + 1;    /* Next search starts after p */

        /* Check if 3p-4 is prime */
        if (is_prime((idx_t)(3 * p - 4))) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)p;
            return;                /* One term added */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for offset 0; return 0 */
    while (cache_len < (size_t)n) find_next_term();
    return cache[n - 1];
}
