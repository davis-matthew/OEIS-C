/*
 * A273052  Numbers n such that 7*n^2 + 8 is a square.
 * https://oeis.org/A273052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2); a(1)=2, a(2)=34
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* below the offset – behaviour irrelevant */

    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t  cache_cap = 0;   /* allocated capacity in elements   */
    static idx_t   cache_len = 0;   /* highest index computed so far    */

    /* grow the cache if it cannot hold up to index n */
    size_t needed = (size_t)(n + 1);   /* we are indexing from 1 and ignore 0 */
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();           /* cannot allocate memory – fail gracefully */
        cache      = tmp;
        cache_cap  = needed;
    }

    /* initialise the first two terms if they are not yet in the cache */
    if (cache_len == 0) {
        cache[1] = (seq_t)2;         /* a(1) */
        cache_len = 1;
    }
    if (n >= 2 && cache_len < 2) {
        cache[2] = (seq_t)34;        /* a(2) */
        cache_len = 2;
    }

    /* compute further terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)16 * cache[i - 1] - cache[i - 2];
    }
    cache_len = n;

    return cache[n];
}
