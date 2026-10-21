/*
 * A248205  Indices of centered octagonal numbers (A016754) that are also
 *          pentagonal numbers (A000326).
 * https://oeis.org/A248205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) Initial values: a(1)=1,
 *            a(2)=50, a(3)=4851 Sequence defined for n>=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of values 1 .. cache_max */
    static seq_t *cache     = NULL;   /* array indexed by n */
    static size_t cache_cap = 0;     /* capacity of cache array */
    static idx_t  cache_max = 0;     /* highest n that has been computed */

    if (n < 1)
        return (seq_t)0;              /* undefined for n<1, return 0 */

    /* ensure the cache is large enough for index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base terms */
    if (cache_max < 1 && n >= 1) {
        cache[1] = (seq_t)1;
        cache_max = 1;
    }
    if (cache_max < 2 && n >= 2) {
        cache[2] = (seq_t)50;
        cache_max = 2;
    }
    if (cache_max < 3 && n >= 3) {
        cache[3] = (seq_t)4851;
        cache_max = 3;
    }

    /* compute further terms with the recurrence */
    if (cache_max < n) {
        idx_t start = cache_max + 1;
        if (start < 4) start = 4;     /* recurrence needs a(i-3) */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)99 * cache[i-1]
                       - (seq_t)99 * cache[i-2]
                       + cache[i-3];
        }
        cache_max = n;
    }

    return cache[n];
}
