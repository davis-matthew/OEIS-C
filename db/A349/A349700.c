/*
 * A349700  Difference between 4*A001590(n+2) and A075092(n).
 * https://oeis.org/A349700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + a(n-4) - a(n-6) with a(0) = -6, a(1)
 *            = 4, a(2) = 6, a(3) = 0, a(4) = 18, a(5) = 24
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array holding a(i) values */
static size_t cache_size = 0;        /* allocated size of 'cache' */
static size_t computed_up_to = 0;    /* number of entries that are already filled */

/* Ensure the cache can hold indices up to 'need' (inclusive) */
static void ensure_cache_size(size_t need)
{
    if (cache_size >= need + 1) return;
    size_t new_cap = cache_size > 0 ? cache_size : 16;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* fatal error – out of memory */
    cache = tmp;
    cache_size = new_cap;
}

/* Initialise base terms (called lazily) */
static void init_base_terms(void)
{
    /* we need indices 0 … 5 at least */
    ensure_cache_size(5);
    cache[0] = -6;
    cache[1] = 4;
    cache[2] = 6;
    cache[3] = 0;
    cache[4] = 18;
    cache[5] = 24;
    computed_up_to = 6;  /* next index to compute is 6 */
}

seq_t a(idx_t n)
{
    /* n is non-negative by specification */
    size_t idx = (size_t)n;

    /* first call – initialise base terms */
    if (!cache) init_base_terms();

    /* if the requested term is already computed, return it */
    if (idx < computed_up_to) return cache[idx];

    /* otherwise extend the cache and compute missing terms */
    ensure_cache_size(idx);

    /* compute terms from computed_up_to up to idx */
    for (size_t i = computed_up_to; i <= idx; ++i)
    {
        if (i < 6) continue; /* base terms already set */
        cache[i] = cache[i-2] + 4 * cache[i-3] + cache[i-4] - cache[i-6];
    }
    computed_up_to = idx + 1;
    return cache[idx];
}
