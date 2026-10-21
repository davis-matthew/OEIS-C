/*
 * A287829  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 6.
 * https://oeis.org/A287829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 2*a(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise base cases */
    if (cache_size == 0) {
        idx_t init_size = 2;
        #ifdef __cplusplus
            cache = new seq_t[init_size];
        #else
            cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        #endif
        if (!cache) return (seq_t)0;                 /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache_size = init_size;
    }

    /* if required value already cached, simply return it */
    if (n < cache_size)
        return cache[n];

    /* grow cache to accommodate up to index n */
    idx_t new_size = n + 1;
    seq_t *new_cache;
    #ifdef __cplusplus
        new_cache = new seq_t[new_size];
    #else
        new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    #endif
    if (!new_cache) return (seq_t)0;                    /* allocation failed */

    /* copy existing values */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        new_cache[i] = (seq_t)9 * new_cache[i - 1] + (seq_t)2 * new_cache[i - 2];
    }

    /* free old cache */
    #ifdef __cplusplus
        delete[] cache;
    #else
        free(cache);
    #endif

    cache = new_cache;
    cache_size = new_size;

    return cache[n];
}
