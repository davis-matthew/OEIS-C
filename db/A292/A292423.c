/*
 * A292423  a(n) = 82*a(n-1) + a(n-2), where a(0) = 0, a(1) = 1.
 * https://oeis.org/A292423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to memoize computed values */
    static seq_t *cache   = NULL;   /* array of seq_t values */
    static idx_t cache_len = 0;     /* current allocated length (cache_len > 0 ⇔ cache non‑NULL) */

    if (n == 0) return 0;
    if (n == 1) return 1;

    /* initialise cache with the two base terms if not yet done */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();          /* out of memory – terminate safely */
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* grow cache to at least n+1 elements and continue the recurrence */
    if (n + 1 > cache_len) {
        /* realloc to the larger size */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory – terminate safely */
        cache = tmp;

        /* compute all missing terms up to index n */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 82 * cache[i - 1] + cache[i - 2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
