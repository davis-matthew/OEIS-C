/*
 * A268858  Prime numbers ending in 39.
 * https://oeis.org/A268858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 150, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A268858: primes ending in 39.
   a(n) is the n-th prime congruent to 39 modulo 100.
   Computed by incremental search using is_prime_big. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cache of found primes ending in 39. */
static seq_t *cache      = NULL;   /* array of primes */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_cnt  = 0;      /* number of primes stored */

/* Next k for candidate 100*k + 39 (k starts at 1). */
static uint64_t next_k = 1;

/* Ensure cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime ending in 39 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Generate candidates until we have at least n primes. */
    while (cache_cnt < n) {
        uint64_t cand64 = 100ULL * next_k + 39ULL;
        seq_t cand = (seq_t)cand64;
        if (is_prime_big(cand)) {
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
