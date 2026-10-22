/*
 * A336626  Triangular numbers that are eight times another triangular number.
 * https://oeis.org/A336626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1154*a(n-2) - a(n-4) + 648 (n≥4) with a(1)=0, a(2)=120,
 *            a(3)=528. (a(0) is needed only for the recurrence)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
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
    static idx_t cache_len = 0;   /* number of stored elements (index 0 .. cache_len-1) */
    static idx_t cache_cap = 0;   /* allocated size of cache */

    if (n == 0) return 0;   /* not used but keeps compiler quiet */

    /* first call – initialise cache with the starting values */
    if (cache_len == 0) {
        cache_cap = (n < 4) ? 4 : n + 1;          /* at least enough for indices 0..3 */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 120;
        cache[3] = 528;
        cache_len = 4;
    }
    /* grow the cache if the requested index is outside the current capacity */
    else if (cache_cap <= n) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    while (cache_len <= n) {
        /* recurrence: a(n) = 1154*a(n-2) - a(n-4) + 648 */
        seq_t val = ((seq_t)1154 * cache[cache_len - 2]) -
                    cache[cache_len - 4] + (seq_t)648;
        cache[cache_len++] = val;
    }

    return cache[n];
}
