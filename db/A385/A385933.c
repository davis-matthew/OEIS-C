/*
 * A385933  Number of ways to tile a "central bump" strip of length n with 1 X
 *          1 squares and 1 X 3 rectangles.
 * https://oeis.org/A385933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) + a(n-6) - a(n-9)
 *            Initial values: a(0)=4, a(1)=9, a(2)=13, a(3)=25, a(4)=30,
 *            a(5)=35, a(6)=52, a(7)=78, a(8)=121 (These are the first nine
 *            terms of the "central bump" tiling sequence.)
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoising previously computed terms */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of computed elements in cache */
    static size_t cap  = 0;   /* allocated capacity of cache */

    /* Hard‑coded initial values for the first nine indices */
    static const seq_t init[9] = {
        (seq_t)4, (seq_t)9, (seq_t)13, (seq_t)25,
        (seq_t)30, (seq_t)35, (seq_t)52, (seq_t)78,
        (seq_t)121
    };

    /* Return cached value if already available */
    if (n < size)
        return cache[(size_t)n];

    /* Resize cache if necessary */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)(cap ? cap << 1 : 9);
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp)
            abort();  /* out of memory */
        for (size_t k = 0; k < size; ++k)
            tmp[k] = cache[k];
        free(cache);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new terms up to index n */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        seq_t val;
        if (i < 9)
            val = init[(idx_t)i];
        else
            val = cache[(size_t)(i-1)] + cache[(size_t)(i-3)]
                - cache[(size_t)(i-4)] + cache[(size_t)(i-5)]
                + cache[(size_t)(i-6)] - cache[(size_t)(i-9)];
        cache[(size_t)i] = val;
    }
    size = (size_t)n + 1;
    return cache[(size_t)n];
}
