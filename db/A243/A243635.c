/*
 * A243635  Number of length n+2 0..4 arrays with no three unequal elements in
 *          a row and new values 0..4 introduced in 0..4 order.
 * https://oeis.org/A243635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 21*a(n-4) - 7*a(n-5) - 6*a(n-6)
 *            with initial values a(1)=4, a(2)=9, a(3)=21, a(4)=51, a(5)=127,
 *            a(6)=324
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation: cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity (indices 0..cache_cap-1) */
    static idx_t cache_max = 0;      /* largest index that has been computed */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 8;  /* enough for indices 0..7 (cache[0] unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;    /* allocation failure */
        /* base values for indices 1..6 */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)21;
        cache[4] = (seq_t)51;
        cache[5] = (seq_t)127;
        cache[6] = (seq_t)324;
        cache_max = 6;
    }

    /* grow the cache if it cannot hold the requested index */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;  /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 6) continue;   /* base values already set */
        seq_t val = (seq_t)7 * cache[i-1]
                  - (seq_t)14 * cache[i-2]
                  + (seq_t)21 * cache[i-4]
                  - (seq_t)7  * cache[i-5]
                  - (seq_t)6  * cache[i-6];
        cache[i] = val;
    }
    cache_max = (cache_max > n ? cache_max : n);

    return cache[n];
}
