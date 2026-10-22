/*
 * A386444  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} k^3 * a(k) * a(n-1-k).
 * https://oeis.org/A386444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386444
 * Recurrence: a(0) = 1;
 *             a(n) = a(n-1) + Σ_{k=0}^{n-1} k^3 * a(k) * a(n-1-k)
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed values           */
static idx_t  cache_size = 0;        /* allocated size of the cache        */
static idx_t  computed_up_to = 0;    /* highest index already computed     */

/* Ensure the cache holds values up to n */
static void ensure_cache(idx_t n)
{
    /* Allocate cache on first use and initialise a(0) */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        computed_up_to = 0;
    }

    /* Already computed all required values */
    if (n <= computed_up_to)
        return;

    /* Grow the array until it can address index n */
    while (cache_size <= n) {
        cache_size <<= 1;                      /* double capacity */
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Append new values */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t kk = (seq_t)k;
            kk *= kk;                          /* kk = k^2      */
            kk *= (seq_t)k;                    /* kk = k^3      */
            seq_t term = kk * cache[k] * cache[i - 1 - k];
            sum += term;
        }
        cache[i] = cache[i - 1] + sum;
    }

    computed_up_to = n;
}

/* public entry point */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
