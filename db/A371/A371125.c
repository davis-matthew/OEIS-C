/*
 * A371125  Number of compositions of 6*n into parts 1 and 6.
 * https://oeis.org/A371125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6), initial values: a(0)=1, a(1)=2, a(2)=9, a(3)=43,
 *            a(4)=196, a(5)=882
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {1, 2, 9, 43, 196, 882};

seq_t a(idx_t n) {
    /* Return pre‑computed values directly for the first six indices. */
    if (n < 6)
        return init_vals[n];

    /* static cache for dynamic programming (memoisation) */
    static seq_t *cache = NULL;          /* array of computed values   */
    static size_t cache_len = 0;         /* number of values stored   */
    static size_t cache_cap = 0;         /* allocated capacity        */

    /* initialise cache if needed */
    if (cache == NULL) {
        /* start with a moderate capacity, at least enough for n+1 */
        size_t init_cap = 16;
        if (init_cap < (size_t)n + 1) init_cap = (size_t)n + 1;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed – unlikely in tests */

        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_len = 6;
        cache_cap = init_cap;
    }

    /* extend the cache up to index n if necessary */
    while (cache_len <= (size_t)n) {
        /* grow the array when full */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failed – unlikely in tests */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute a[cache_len] using the 6th‑order linear recurrence */
        size_t i = cache_len;
        seq_t pos = 7 * cache[i-1] + 20 * cache[i-3] + 6 * cache[i-5];
        seq_t neg = 15 * cache[i-2] + 15 * cache[i-4] + cache[i-6];
        cache[i] = pos - neg;

        ++cache_len;
    }

    return cache[n];
}
