/*
 * A272931  a(n) = 2^(n+1)*cos(n*arctan(sqrt(15))).
 * https://oeis.org/A272931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 4*a(n-2); a(0)=2, a(1)=1
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 63, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;          /* highest index constructed */
    static size_t cache_cap = 0;          /* allocated size */

    /* Helper to grow cache capacity */
    if (cache_last == 0 && n == 0) { /* first call, initialise a(0), a(1) */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, return 0 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache_last = 1;
    }

    /* Ensure cache covers index n */
    if (n > cache_last) {
        /* grow capacity */
        if (cache_cap == 0) cache_cap = 2;
        while ((idx_t)cache_cap <= n) cache_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure */
        cache = new_cache;

        /* compute from cache_last+1 up to n */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = cache[i-1] - (seq_t)4 * cache[i-2];
        }
        cache_last = n;
    }

    return cache[n];
}
