/*
 * A298681  Start with the square tile of the Shield tiling and recursively
 *          apply the substitution rule. a(n) is the number of triangles with
 *          6 markings after n iterations.
 * https://oeis.org/A298681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=4, a(2)=4, a(3)=32;
 *            a(n)=3*a(n-1)+5*a(n-2)-9*a(n-3)+2*a(n-4) for n>3 (OEIS A298681 –
 *            number of triangles with 6 markings after n iterations)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;  /* current memory capacity */
    static idx_t cache_len = 0;  /* number of terms stored (last valid index +1) */

    /* initialise cache if needed */
    if (cache == NULL) {
        /* allocate a modest capacity to accommodate initial terms */
        idx_t cap = (n + 1 > 4) ? n + 1 : 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        /* store fixed initial values */
        cache[0] = 0;
        cache[1] = 4;
        cache[2] = 4;
        cache[3] = 32;
        cache_cap = cap;
        cache_len = 4;
    }

    /* if n is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* enlarge capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 3) {
            /* already initialized; skip */
            continue;
        }
        /* recurrence: a(i) = 3*a(i-1) + 5*a(i-2) - 9*a(i-3) + 2*a(i-4) */
        cache[i] = 3 * cache[i - 1] + 5 * cache[i - 2] - 9 * cache[i - 3] + 2 * cache[i - 4];
    }
    cache_len = n + 1;

    return cache[n];
}
