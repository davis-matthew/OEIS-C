/*
 * A351530  The number of quinary strings of length n containing 00.
 * https://oeis.org/A351530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 16*a(n-2) - 20*a(n-3) Initial values: a(0)=0,
 *            a(1)=0, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to memoise computed values */
    static seq_t *cache = NULL;        /* array of computed terms */
    static size_t cache_len = 0;       /* capacity of the cache */
    static idx_t  last_computed = 2;   /* highest n for which cache[i] is valid */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* enough space for the requested n plus at least 3 base terms */
        cache_len = (size_t)((n + 1) > 3 ? (n + 1) : 3);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 (unused here) */
        cache[0] = 0;  /* a(0) */
        cache[1] = 0;  /* a(1) */
        cache[2] = 1;  /* a(2) */
    }

    /* if the needed value is already cached, return it immediately */
    if (n <= last_computed) {
        return cache[n];
    }

    /* ensure sufficient capacity in the cache */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = cache_len * 2;
        while (new_len <= (size_t)n) {
            new_len *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* reallocation failure – return 0 (unused here) */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute and cache all missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t = 9 * cache[i - 1];
        t -= 16 * cache[i - 2];
        t -= 20 * cache[i - 3];
        cache[i] = t;
    }

    last_computed = n;
    return cache[n];
}
