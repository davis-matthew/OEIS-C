/*
 * A265430  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          188" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n>4, with a(0)=0, a(1)=1,
 *            a(2)=3, a(3)=3, a(4)=5
 * Range:     u8 n <= 204, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL; /* holds computed terms */
    static idx_t cache_size  = 0;     /* number of elements in cache */

    /* initialise cache with the first five values */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
    }

    /* if the value has already been computed, return it */
    if (n < cache_size)
        return cache[n];

    /* extend the cache to hold up to index n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;   /* allocation failure */

    cache = tmp;
    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
    }
    cache_size = n + 1;

    return cache[n];
}
