/*
 * A386347  Primes without {4, 7} as digits.
 * https://oeis.org/A386347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 2224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: successively test integers for primality and digit restriction,
   storing qualifying primes in a static cache.  a(1) = 2, a(2) = 3,
   a(3) = 5, ...  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"    /* bool is_prime(idx_t x); */

static seq_t   *cache      = NULL;   /* cached qualifying primes   */
static size_t  cache_size = 0;       /* number of elements stored */
static size_t  cache_cap  = 0;       /* allocated array capacity    */
static uint64_t last_val  = 1;       /* last integer examined        */

/* Append new value to cache, growing array if necessary */
static void
cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Check that a decimal integer contains no digits 4 or 7 */
static bool
digits_ok(uint64_t x)
{
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d == 4 || d == 7)
            return false;
        x /= 10;
    }
    return true;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) {               /* offset is 1; return 0 for safety */
        return 0;
    }

    /* Pre‑populate the cache with the first qualifying prime */
    if (cache_size == 0) {
        cache_append((seq_t)2);
        last_val = 2;
    }

    /* Extend cache until it contains the n‑th prime */
    while (cache_size < (size_t)n) {
        uint64_t cand = last_val + 1;          /* start searching after last found prime */
        while (true) {
            if (is_prime(cand) && digits_ok(cand)) {
                cache_append((seq_t)cand);
                last_val = cand;
                break;
            }
            cand++;
        }
    }

    return cache[(size_t)n - 1];
}
