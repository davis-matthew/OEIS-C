/*
 * A396582  a(n) = 4*a(n - 2) - 2*a(n - 1) for n >= 2, a(0) = 1, a(1) = 6.
 * https://oeis.org/A396582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-2) - 2*a(n-1) for n>=2, a(0) = 1, a(1) = 6
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of values stored (n+1) */

    /* if already computed, return immediately */
    if (n < cache_len)
        return cache[n];

    /* compute up to n */
    size_t new_len = (size_t)n + 1;           /* required array size */
    size_t old_len = cache_len;              /* current size before grow */

    /* grow the cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) abort();                 /* fatal error */
        cache[0] = (seq_t)1;
        if (new_len > 1)
            cache[1] = (seq_t)6;
    } else if (new_len > old_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise missing base cases when expanding from an empty array */
        if (old_len < 1)
            cache[0] = (seq_t)1;
        if (old_len < 2 && new_len > 1)
            cache[1] = (seq_t)6;
    }

    /* compute any new entries */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i < 2) continue;                        /* base values already set */
        cache[i] = (seq_t)4 * cache[i-2] - (seq_t)2 * cache[i-1];
    }

    cache_len = new_len;        /* update current size */
    return cache[n];
}
