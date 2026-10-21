/*
 * A279682  The maximum number of coins that can be processed in n weighings
 *          where all coins are real except for one LHR-coin.
 * https://oeis.org/A279682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) - 8*a(n-5)
 * Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=19, a(4)=49, a(5)=123
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t i;

    /* initialise the cache with the first six terms */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 9;
        cache[3] = 19;
        cache[4] = 49;
        cache[5] = 123;
        cache_size = 6;
    }

    /* if already computed, return the value */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend the cache to include n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) return 0; /* allocation failure */
    cache = new_cache;

    /* compute new terms using the recurrence */
    for (i = cache_size; i <= n; ++i) {
        /* recurrence uses terms i-1 .. i-5 (i >= 5) */
        cache[i] = 3 * cache[i - 1]
                 - cache[i - 2]
                 + cache[i - 3]
                 - 2 * cache[i - 4]
                 - 8 * cache[i - 5];
    }
    cache_size = n + 1;
    return cache[n];
}
