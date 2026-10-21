/*
 * A250809  Number of (n+1) X (5+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3); Initial values:
 *            a(1)=784, a(2)=3097, a(3)=11116
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 34, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* External definition of seq_t and idx_t provided by the build. */

seq_t a(idx_t n)
{
    if (n == 0) /* undefined index, but must return some value */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* highest computed index */

    /* allocate enough space for indices 0 .. n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialise first three terms if this is the first call */
    if (cache_len == 0) {
        /* store indexes 0..3, need at least capacity 4 */
        if (cache_cap < 4) {
            seq_t *tmp = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = 4;
        }
        cache[0] = 0;                 /* unused */
        cache[1] = 784;                /* a(1) */
        cache[2] = 3097;               /* a(2) */
        cache[3] = 11116;              /* a(3) */
        cache_len = 3;                 /* highest computed index */
    }

    /* If the requested index has already been computed, return it */
    if (n <= cache_len)
        return cache[n];

    /* Compute terms up to n using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 1]
                 - (seq_t)11 * cache[i - 2]
                 + (seq_t)6 * cache[i - 3];
    }
    cache_len = (size_t)n;
    return cache[n];
}
