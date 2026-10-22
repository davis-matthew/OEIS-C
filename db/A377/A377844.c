/*
 * A377844  Numbers that have a single odd exponent larger than 1 in their
 *          prime factorization.
 * https://oeis.org/A377844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 7535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377844 – numbers with exactly one odd prime exponent larger than 1
   in their prime factorization.  
   The implementation uses a prime sieve to generate primes up to a fixed limit
   and scans integers in increasing order, testing each one by trial division.
   The results are cached so repeated calls with arbitrary indices are fast. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------ static helpers for prime sieve ----------------------------------- */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialised */
    const size_t limit = 1000000;         /* primes up to 1e6 -> 78498 primes */

    /* Sieve of Eratosthenes */
    bool *mark = (bool *)malloc(limit + 1);
    memset(mark, 1, limit + 1);
    mark[0] = mark[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (mark[i])
            for (size_t j = i * i; j <= limit; j += i) mark[j] = 0;

    /* Count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (mark[i]) ++primes_cnt;

    primes_cap = primes_cnt;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (mark[i]) primes[idx++] = (uint32_t)i;

    free(mark);
}

/* ------ test whether an integer has exactly one odd exponent >1 ----------- */
static bool has_single_odd_exp_gt_one(uint64_t n)
{
    int odd_gt1 = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= rem; ++i) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            int e = 0;
            do {
                ++e;
                rem /= p;
            } while (rem % p == 0);
            if ((e & 1) && e > 1) {
                ++odd_gt1;
                if (odd_gt1 > 1) return false;
            }
        }
    }
    /* remaining rem is 1 or a prime > sqrt(initial n), exponent 1 (odd but not >1) */
    return odd_gt1 == 1;
}

/* ------ caching for the sequence ------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;   /* start scanning from 2 */

/* Grow cache capacity if needed */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* ------ public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */
    if (cache == NULL) {           /* first call - initialise */
        init_primes();
        cache = (seq_t *)malloc(32 * sizeof(seq_t));
        cache_cap = 32;
        cache_len = 0;
        next_candidate = 2;
    }

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        while (true) {
            uint64_t candidate = next_candidate++;
            if (has_single_odd_exp_gt_one(candidate)) {
                ensure_cache_capacity();
                cache[cache_len++] = (seq_t)candidate;
                break;
            }
        }
    }
    return cache[(size_t)n - 1];
}
