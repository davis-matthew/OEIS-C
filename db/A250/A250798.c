/*
 * A250798  Number of (1+1) X (n+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) with initial values a(1)=10,
 *            a(2)=24, a(3)=54 Recurrence holds for all n >= 4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* ensure the cache can hold up to index n */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();                 /* out of memory */
        if (cache) {
            for (idx_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1)
            cache[0] = (seq_t)10;
        else if (i == 2)
            cache[1] = (seq_t)24;
        else if (i == 3)
            cache[2] = (seq_t)54;
        else {
            /* recurrence: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) */
            seq_t term1 = cache[i-2] * (seq_t)3;
            seq_t term2 = cache[i-3];
            seq_t term3 = cache[i-4] * (seq_t)2;
            cache[i-1] = term1 - term2 - term3;
        }
    }

    return cache[n-1];
}
