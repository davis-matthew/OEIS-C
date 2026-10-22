/*
 * A332348  Number of maximal independent sets in the 3 X n king graph.
 * https://oeis.org/A332348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) (n ≥ 6) Initial values: a(0) = 1, a(1) = 2, a(2) = 6,
 *            a(3) = 8, a(4) = 22, a(5) = 40
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* memoisation array   */
static size_t cache_size = 0;      /* allocated elements  */
static idx_t  computed   = (idx_t)-1; /* highest index computed */

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cache == NULL) {
        cache_size = 8;                     /* at least 8 slots          */
        cache      = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed         */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 6;
        cache[3] = 8;
        cache[4] = 22;
        cache[5] = 40;
        computed = 5;
    }

    /* Return already computed value */
    if ((idx_t)n <= computed)
        return cache[n];

    /* Ensure capacity up to n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;       /* grow in powers of two */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed     */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute values from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1]
                 + (seq_t)2 * cache[i-2]
                 + cache[i-3]
                 - (seq_t)2 * cache[i-4]
                 + (seq_t)2 * cache[i-5]
                 - (seq_t)2 * cache[i-6];
    }
    computed = n;
    return cache[n];
}
