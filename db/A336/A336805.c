/*
 * A336805  a(n) = (n!)^2 * Sum_{k=0..n} 3^(n-k) / (k!)^2.
 * https://oeis.org/A336805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3*n^2*a(n-1) + 1, for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence given in the OEIS entry. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation, grows as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_limit = 0;   /* number of entries stored, i.e. cache_limit == last_index + 1 */

    /* Initialise cache with a(0) if empty. */
    if (cache_limit == 0) {
        cache_limit = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
    }

    /* If requested index is beyond current cache, extend it. */
    if (n >= cache_limit) {
        /* Choose new allocation size: at least twice current, or just reach n+1. */
        size_t new_alloc = (size_t)(n + 1);
        if (new_alloc < cache_limit) new_alloc = cache_limit;
        else if (new_alloc < 2 * cache_limit) new_alloc = 2 * cache_limit;

        seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* reallocation failed */
        cache = new_cache;

        /* Compute missing entries using the recurrence. */
        for (idx_t i = cache_limit; i <= n; ++i) {
            seq_t prev    = cache[i - 1];
            seq_t factor  = (seq_t)3 * (seq_t)i * (seq_t)i;  /* 3*i^2 */
            cache[i] = factor * prev + (seq_t)1;
        }
        cache_limit = n + 1;
    }

    return cache[n];
}
