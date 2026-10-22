/*
 * A364319  a(n) = (A077446(n) + 1)/2 for n >= 0.
 * https://oeis.org/A364319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) - 2, with a(0)=0, a(1)=1, a(2)=3,
 *            a(3)=6
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t max_computed = 3; /* last index for which a() is known */

    /* initialisation: set first four terms */
    if (cache == NULL) {
        cache_len = 8; /* start with some space */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure; unlikely */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = 0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = cache_len;
        while ((size_t)n + 1 > new_len) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* reallocation failure; unlikely */
        cache = tmp;
        /* zero new entries (they are small, no overflow issue) */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* if already computed up to n, return cached value */
    if (n <= max_computed) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)6 * cache[i - 2] - cache[i - 4] - (seq_t)2;
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
