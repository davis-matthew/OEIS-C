/*
 * A320965  Squares divisible by a single cube > 1.
 * https://oeis.org/A320965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A060687(n))^2
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_a060687_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060687(n) is the nth positive integer having exactly one prime factor
   raised to the power 2 and all other prime factors raised to the first
   power.  This implementation generates A060687(n) on demand and stores
   it in a dynamic cache so that subsequent calls are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_a060687_candidate.h"

/* Cache for the base integers A060687(n).  Elements are stored in
   ascending order.  The cache grows dynamically as more terms are
   requested. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */
static size_t cache_cap = 0;   /* capacity of the cache array */
static uint64_t next_candidate = 2;   /* next integer to test */

/* Increase the capacity of the cache array.  The new capacity is at least
   'min_cap'.  If the cache is currently empty, a small initial capacity is
   chosen. */
static void enlarge_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the unlikely event of OOM we simply keep the old cache; the
       function will return 0 for any further request. */
    if (!new_cache)
        return;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least 'n' elements (0‑based index
   n-1).  This function expands the cache by scanning successive
   candidates until enough terms are found. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    while (cache_len < n) {
        /* Find the next candidate with the desired property. */
        while (!is_a060687_candidate(next_candidate))
            ++next_candidate;

        /* Add it to the cache. */
        enlarge_cache(cache_len + 1);
        /* The cache may be NULL if enlarge_cache failed; in that case
           we simply drop the value. */
        if (cache != NULL)
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Public sequence function.  Off‑by‑one as required: a(1) is the first
   term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; undefined for 0 */
        return 0;
    ensure_cache(n);
    if (cache_len < n || cache == NULL)   /* out of memory or request too large */
        return 0;
    seq_t m = cache[n - 1];
    return (seq_t)m * (seq_t)m;   /* exact square in seq_t type */
}
