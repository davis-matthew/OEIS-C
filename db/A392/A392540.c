/*
 * A392540  Expansion of 1 / ((1-x)^2 - x^5).
 * https://oeis.org/A392540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) Initial values: a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=5
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 104,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n-th term of A392540.  The function is memoized so that
   calls can happen in any order.  It is safe for n up to at least 10000. */
seq_t a(idx_t n) {
    /* Direct return for the first five terms. */
    if (n < 5) {
        static const seq_t init[5] = {1, 2, 3, 4, 5};
        return init[n];
    }

    /* Static cache for previously computed values. */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* number of initialized terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* OOM: unlikely in test harness */

        /* Seed initial values. */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 4;
        cache[4] = 5;

        cache_len = 5;
    }

    /* Extend the allocated buffer if necessary. */
    if ((size_t)n >= cache_cap) {
        cache_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* OOM */
        cache = tmp;
    }

    /* Compute missing terms up to n. */
    if ((size_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t tmp = cache[i - 1] + cache[i - 1];   /* 2*a(i-1) */
            tmp = tmp - cache[i - 2];                 /* -a(i-2) */
            cache[i] = tmp + cache[i - 5];            /* +a(i-5) */
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
