/*
 * A306161  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A306161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = a(n-1) + 2*a(n-2) + 5*a(n-3) + 2*a(n-4)
          - 2*a(n-5) - 8*a(n-6) - 8*a(n-7)   for n>10
   Initial values:
      a(1)=4, a(2)=5, a(3)=17, a(4)=32, a(5)=77,
      a(6)=207, a(7)=523, a(8)=1304, a(9)=3307, a(10)=8414 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;
    static int initialized   = 0;

    /* initialize the cache with the first 10 terms */
    if (!initialized) {
        cache_len = 10;                                   /* highest cached index */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!cache) return 0;                            /* allocation failure */

        cache[0] = 0;   /* unused to keep indices matching OEIS */
        cache[1] = 4;   cache[2] = 5;   cache[3] = 17;
        cache[4] = 32;  cache[5] = 77;  cache[6] = 207;
        cache[7] = 523; cache[8] = 1304; cache[9] = 3307;
        cache[10] = 8414;
        initialized = 1;
    }

    if (n == 0) return 0;                  /* offset starts at 1 */

    /* if n is already in the cache, return it */
    if ((idx_t)cache_len >= n) {
        return cache[(size_t)n];
    }

    /* enlarge the cache to at least n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!tmp) return 0;                    /* allocation failure */
    cache = tmp;
    cache_len = new_len;

    /* compute missing terms (from old_len+1 up to n) using the recurrence */
    for (size_t i = old_len + 1; i <= (size_t)n; ++i) {
        seq_t term = cache[i-1]
                   + (seq_t)2 * cache[i-2]
                   + (seq_t)5 * cache[i-3]
                   + (seq_t)2 * cache[i-4];
        term -= (seq_t)2 * cache[i-5];
        term -= (seq_t)8 * cache[i-6];
        term -= (seq_t)8 * cache[i-7];
        cache[i] = term;
    }

    return cache[(size_t)n];
}
