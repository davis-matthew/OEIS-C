/*
 * A336749  Number of circular arrangements of the first n positive integers
 *          such that adjacent terms have absolute difference 1 or 4.
 * https://oeis.org/A336749
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + a(n-3) + a(n-4) + 2*a(n-5) + 2*a(n-6) + a(n-7)
 *            + a(n-8) + a(n-9) (for n > 13) Initial values: a(5)=1, a(6)=0,
 *            a(7)=1, a(8)=1, a(9)=1, a(10)=3, a(11)=2, a(12)=3, a(13)=6 a(n)
 *            is defined for every n ≥ 5
 * Range:     u8 n <= 26, u16 n <= 44, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i ≥ 0.  Indices <5 are unused. */
    static seq_t *cache     = NULL;   /* pointer to the cache array   */
    static size_t cache_cap = 0;      /* allocated size (elements)    */
    static idx_t  max_computed = 4;   /* highest index whose value
                                        is known (initially 4)       */

    /* Handle indices below the offset: they are undefined (return 0). */
    if (n < 5)
        return 0;

    /* First time: allocate space for indices 0..13 at least. */
    if (cache_cap == 0) {
        cache_cap = 14;                      /* indices 0..13 inclusive   */
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                       /* allocation failed         */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;                   /* zero initial content      */

        /* Initialise the known values up to n = 13. */
        cache[5]  = 1;
        cache[6]  = 0;
        cache[7]  = 1;
        cache[8]  = 1;
        cache[9]  = 1;
        cache[10] = 3;
        cache[11] = 2;
        cache[12] = 3;
        cache[13] = 6;
        max_computed = 13;
    }

    /* Grow the cache if necessary.  Ensure space for index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                       /* allocation failed         */
        /* Zero any newly allocated elements. */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Return the value if it is already computed. */
    if (n <= max_computed)
        return cache[n];

    /* Compute the recurrence for missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence is valid for i ≥ 14, and i‑3 … i‑9 are all ≥ 5. */
        seq_t val = cache[i - 3] + cache[i - 4]
                  + 2 * cache[i - 5] + 2 * cache[i - 6]
                  + cache[i - 7] + cache[i - 8] + cache[i - 9];
        val -= cache[i - 1];                /* guaranteed non‑negative */
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
