/*
 * A386128  Primes having only {1, 4, 8, 9} as digits.
 * https://oeis.org/A386128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes having only {1,4,8,9} as digits (OEIS A386128) */
/* The implementation searches successive integers, keeps a cache of
   found primes and returns the n‑th one.  Only the built‑in helper
   functions for memory management of seq_t are used, so it works
   with arbitrary‑precision values as well as with native integer types. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_t *seq_alloc_new_array(size_t n) */
#include "../utils/seq_realloc.h"           /* void *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

static /* cache of sequence values */
seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* capacity of cache array */

/* next integer to test;   1 is the smallest with allowed digits */
static uint64_t candidate = 1;

/* small prime sieve for trial division */
static uint64_t *small_primes = NULL;
static size_t small_primes_cnt = 0;

static void init_small_primes(void)
{
    if (small_primes != NULL)
        return;                        /* already initialized */

    const size_t LIMIT = 100000;         /* sufficient for n ≤ 10000          */
    bool *composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    size_t cap = 20000;
    small_primes = (uint64_t *)malloc(cap * sizeof(uint64_t));

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!composite[i]) {
            if (small_primes_cnt == cap) {
                cap *= 2;
                small_primes = (uint64_t *)realloc(small_primes,
                                                   cap * sizeof(uint64_t));
            }
            small_primes[small_primes_cnt++] = i;
            if (i * i > LIMIT)
                continue;
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                composite[j] = true;
        }
    }
    free(composite);
}

/* return true iff n is a prime using the precomputed small primes */
static bool is_prime_small(uint64_t n)
{
    if (n < 2)
        return false;
    init_small_primes();
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* the digits of n must belong to {1,4,8,9} */
static bool allowed_digits(uint64_t n)
{
    while (n > 0) {
        uint64_t d = n % 10;
        if (d != 1 && d != 4 && d != 8 && d != 9)
            return false;
        n /= 10;
    }
    return true;
}

/* append a new term to the cache, resizing if necessary */
static void add_to_cache(uint64_t term)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0 ? 32 : cache_cap * 2);
        seq_t *new_cache = (seq_t *)seq_realloc((seq_t *)cache, cache_cap, new_cap);
        if (new_cache == NULL) {
            /* In the unlikely event of memory exhaustion; abort */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)term;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1 – return 0 for safety */
        return (seq_t)0;

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Find next candidate satisfying digit and primality tests */
        while (true) {
            if (allowed_digits(candidate) && is_prime_small(candidate)) {
                add_to_cache(candidate);
                ++candidate;
                break;
            }
            ++candidate;
        }
    }
    return cache[(size_t)n - 1];
}
