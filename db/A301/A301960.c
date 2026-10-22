/*
 * A301960  Number of nX4 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) (n>4) a(1) = 5, a(2) = 9,
 *            a(3) = 25, a(4) = 64
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* defined only for n >= 1 */

    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static idx_t max_computed = 0;

    /* enlarge cache if necessary */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */

        /* initialise new slots to zero */
        for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = 0;
        new_cache[0] = 0;          /* sentinel for unused index 0 */

        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    if (max_computed < n) {
        idx_t start = max_computed + 1;
        if (start < 1) start = 1;

        for (idx_t i = start; i <= n; ++i) {
            if (i == 1)       cache[i] = 5;
            else if (i == 2)  cache[i] = 9;
            else if (i == 3)  cache[i] = 25;
            else if (i == 4)  cache[i] = 64;
            else              cache[i] = 2 * cache[i-1] + 2 * cache[i-2] - cache[i-3];
        }
        max_computed = n;
    }

    return cache[n];
}
