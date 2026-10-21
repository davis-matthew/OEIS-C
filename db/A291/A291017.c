/*
 * A291017  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 5 S + S^2.
 * https://oeis.org/A291017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) for n >= 3 Initial values: a(0)=5,
 *            a(1)=29, a(2)=168
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the first three values directly. */
    if (n == (idx_t)0) return (seq_t)5;
    if (n == (idx_t)1) return (seq_t)29;
    if (n == (idx_t)2) return (seq_t)168;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;         /* ptr to cached values */
    static idx_t   cache_len = 0;       /* highest index computed so far */
    static size_t  cache_cap = 0;       /* allocated capacity */

    size_t needed_cap = (size_t)n + 1;
    if (cache == NULL) {
        /* First allocation: ensure space for at least the first three terms */
        if (needed_cap < 3) needed_cap = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * needed_cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        /* Initialise the cache with the first three values. */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)168;
        cache_cap = needed_cap;
        cache_len = 2;   /* we have values up to index 2 */
    }
    else if (needed_cap > cache_cap) {
        /* Grow the cache to accommodate the requested index. */
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed_cap) new_cap = needed_cap;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute any missing values up to index n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1] - (seq_t)7 * cache[i - 2];
    }
    cache_len = n;

    return cache[n];
}
