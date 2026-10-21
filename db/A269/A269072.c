/*
 * A269072  Number of nX5 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) -198*a(n-2) +584*a(n-3) +137*a(n-4)
 *            -2864*a(n-5) +1132*a(n-6) +4336*a(n-7) -1391*a(n-8) -2280*a(n-9)
 *            +90*a(n-10) +200*a(n-11) -25*a(n-12) Initial values: a(1)=23,
 *            a(2)=185, a(3)=1887, a(4)=17713, a(5)=165607, a(6)=1529241,
 *            a(7)=14011359, a(8)=127528641, a(9)=1154377943, a(10)=104...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                          /* undefined, return 0 */

    /* static cache of already computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first 12 terms */
    static const seq_t init[12] = {
        (seq_t)23, (seq_t)185, (seq_t)1887, (seq_t)17713,
        (seq_t)165607, (seq_t)1529241, (seq_t)14011359,
        (seq_t)127528641, (seq_t)1154377943, (seq_t)10400164377,
        (seq_t)93314875007, (seq_t)834244316721
    };

    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
    }

    /* if already computed, return immediately */
    if (n <= cache_size)
        return cache[n - 1];

    /* extend the cache up to n */
    idx_t old_size = cache_size;
    cache_size = n;
    seq_t *new_cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;      /* reallocation failed */
    cache = new_cache;

    /* compute the missing terms using the recurrence */
    for (idx_t i = old_size; i < n; ++i) {    /* i is 0‑based for a(i+1) */
        /* compute sums of positive and negative contributions separately
           to avoid underflow on unsigned seq_t types. */
        seq_t sum_pos = (seq_t)24  * cache[i - 1]          /* a(i)   */
                       + (seq_t)584 * cache[i - 3]          /* a(i-2) */
                       + (seq_t)137 * cache[i - 4]          /* a(i-3) */
                       + (seq_t)1132 * cache[i - 6]         /* a(i-5) */
                       + (seq_t)4336 * cache[i - 7]         /* a(i-6) */
                       + (seq_t)90  * cache[i -10]         /* a(i-9) */
                       + (seq_t)200 * cache[i -11];        /* a(i-10) */

        seq_t sum_neg = (seq_t)198  * cache[i - 2]          /* a(i-1) */
                       + (seq_t)2864 * cache[i - 5]          /* a(i-4) */
                       + (seq_t)1391 * cache[i - 8]          /* a(i-7) */
                       + (seq_t)2280 * cache[i - 9]          /* a(i-8) */
                       + (seq_t)25   * cache[i -12];        /* a(i-11) */

        cache[i] = sum_pos - sum_neg;
    }

    return cache[n - 1];
}
