/*
 * A279245  Number of subsets of {1, 2, 3, ..., n} that include no consecutive
 *          odd integers.
 * https://oeis.org/A279245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-4), a(0)=1, a(1)=2, a(2)=4, a(3)=6
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static idx_t cache_len = 0;      /* number of valid elements (>= cache_len-1) */

    /* Ensure the cache has at least n+1 elements. */
    if (cache_cap <= n)
    {
        idx_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;          /* at least space for the 4 base values */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms incrementally up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)       cache[i] = (seq_t)1;
        else if (i == 1)  cache[i] = (seq_t)2;
        else if (i == 2)  cache[i] = (seq_t)4;
        else if (i == 3)  cache[i] = (seq_t)6;
        else
            cache[i] = ( (seq_t)2 * cache[i-2] ) + ( (seq_t)4 * cache[i-4] );
    }

    cache_len = n + 1;
    return cache[n];
}
