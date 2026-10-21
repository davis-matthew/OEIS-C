/*
 * A298189  Number of nX2 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -2*a(n-2) +2*a(n-3) -6*a(n-4) -4*a(n-5); Initial
 *            values: a(1)=2, a(2)=8, a(3)=26, a(4)=88, a(5)=298
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑indexed cache; cache[0] is unused.
       cache_size is the highest valid index (capacity‑1).  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* last valid index that is allocated */
    static idx_t computed   = 0;   /* highest index already computed */

    if (n < 1)
        return 0;   /* sequence is defined only for n ≥ 1 */

    /* Grow the cache to accommodate index n (if necessary). */
    if (n > cache_size) {
        idx_t new_cap = n + 1;            /* amount of slots needed (0..n) */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;    /* allocation failure */

        /* Copy existing values.  Cache may be NULL on first call. */
        if (cache != NULL && computed > 0) {
            for (idx_t i = 1; i <= computed; ++i)
                new_cache[i] = cache[i];
        }

        /* Initialise the freshly allocated slots to 0. */
        for (idx_t i = computed + 1; i <= n; ++i)
            new_cache[i] = 0;

        /* Release the old buffer. */
        free(cache);
        cache = new_cache;
        cache_size = n;   /* last valid index now is n */
    }

    /* If the requested value is already known, return it. */
    if (n <= computed)
        return cache[n];

    /* Compute successive terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)8;
        } else if (i == 3) {
            cache[i] = (seq_t)26;
        } else if (i == 4) {
            cache[i] = (seq_t)88;
        } else if (i == 5) {
            cache[i] = (seq_t)298;
        } else {
            /* Compute  a(i) = 4*a(i-1) -2*a(i-2) +2*a(i-3) -6*a(i-4) -4*a(i-5)  */
            seq_t t = 4 * cache[i-1];
            t += 2 * cache[i-3];
            t -= (2 * cache[i-2] + 6 * cache[i-4] + 4 * cache[i-5]);
            cache[i] = t;
        }
    }

    computed = n;
    return cache[n];
}
