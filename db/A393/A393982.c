/*
 * A393982  a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 1, a(1) = 7, a(2) = 49,
 *          a(3) = 56.
 * https://oeis.org/A393982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), with * a(0) = 1, a(1) = 7, a(2)
 *            = 49, a(3) = 56 * (recurrence up to offset 0)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* We only use the types seq_t and idx_t given by the build system.
 * No other type definitions or headers are required. */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 4;                 /* enough for indices 0..3 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)56;
        cache_len = 4;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_len) {
        /* ensure we have space for indices 0..n */
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)(n + 1))
                new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();  /* unlikely in test harness */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[(size_t)(i - 1)];
            val += cache[(size_t)(i - 3)];
            val += cache[(size_t)(i - 4)];
            cache[(size_t)i] = val;
        }
        cache_len = (size_t)(n + 1);
    }

    return cache[(size_t)n];
}
