/*
 * A317809  Number of nX2 0..1 arrays with every element unequal to 0, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5) - a(n-6)
 *            (empirical recurrence given in the OEIS entry)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence starts at index 1. For n < 1 the function returns 0. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Static state for memoisation. */
    static seq_t *cache = NULL;   /* array of computed values          */
    static idx_t capacity = 0;    /* allocated size of cache (terms)   */
    static idx_t cached   = 0;    /* number of terms actually stored   */

    /* Initialise table with the first six terms. */
    if (cached == 0) {
        /* We need space for at least six terms. */
        capacity = 6;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)2;   /* a(3) */
        cache[3] = (seq_t)3;   /* a(4) */
        cache[4] = (seq_t)5;   /* a(5) */
        cache[5] = (seq_t)10;  /* a(6) */
        cached = 6;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cached) return cache[n-1];

    /* Make sure the cache is large enough to store up to index n. */
    if (n > capacity) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        cache = new_cache;
        capacity = (idx_t)new_cap;
    }

    /* Compute new terms using the recurrence. */
    /* cached is the number of terms stored so far (a(cached)). */
    for (idx_t i = cached; i < n; ++i) {
        /* i corresponds to a(i+1) (0‑based index). */
        cache[i] = ( (seq_t)2 * cache[i-1] )
                 + cache[i-2]
                 + cache[i-3]
                 - cache[i-4]
                 - cache[i-5]
                 - cache[i-6];
    }
    cached = n;
    return cache[n-1];
}
