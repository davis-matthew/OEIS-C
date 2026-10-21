/*
 * A291772  Number of minimal dominating sets in the 2n-crossed prism graph.
 * https://oeis.org/A291772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 7*a(n-3) + 17*a(n-4) + 2*a(n-5) for
 *            n > 5 with a(1)=4, a(2)=12, a(3)=61, a(4)=316, a(5)=1304
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values for n = 1..5 */
    static const seq_t init[6] = {0, (seq_t)4, (seq_t)12, (seq_t)61, (seq_t)316, (seq_t)1304};

    if (n <= 5)
        return init[n];

    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;            /* one past last valid index in cache */

    /* first time: create cache with index 0..5 */
    if (cache_size == 0) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return init[5];        /* allocation failed: fall back to known value */
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = init[i];
    }

    /* extend cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return init[5];          /* allocation failed: return known value */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                (seq_t)4 * cache[i-1]
              - (seq_t)2 * cache[i-2]
              + (seq_t)7 * cache[i-3]
              + (seq_t)17 * cache[i-4]
              + (seq_t)2 * cache[i-5];
        }
        cache_size = new_size;
    }

    return cache[n];
}
