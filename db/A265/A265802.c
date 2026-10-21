/*
 * A265802  Coefficient of x^2 in minimal polynomial of the continued fraction
 *          [1^n,4,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A265802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) (a(0)=1, a(1)=11, a(2)=19,
 *            a(3)=59)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;          /* pointer to cached values */
    static idx_t cache_size = 0;         /* current allocated size */
    static idx_t max_n = (idx_t)-1;      /* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* store values for n = 0,1,2,3 */
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)19;
        cache[3] = (seq_t)59;
        max_n = 3;
    }

    /* return stored value if already known */
    if (n <= max_n) {
        return cache[n];
    }

    /* grow cache if needed to hold indices up to n */
    if ((size_t)(n + 1) > (size_t)cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – fall back to computing without cache */
            cache = NULL;
        } else {
            cache = tmp;
            cache_size = (idx_t)new_size;
        }
    }

    /* compute missing values iteratively */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1]
                 + (seq_t)2 * cache[i - 2]
                 - cache[i - 3];
    }
    max_n = n;
    return cache[n];
}
