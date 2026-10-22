/*
 * A393682  Lower (1/3,1/6) midsequence of (n^2) and ((n+2)^2); see Comments.
 * https://oeis.org/A393682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + a(n-8); with
 *            (a(0),...,a(7)) = (0,1,4,7,11,16,22,29)
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms              */
    static idx_t   cap  = 0;      /* allocated capacity of cache          */
    static idx_t   len  = 0;      /* number of terms that have been filled */

    /* First call: allocate the cache and store the 8 known initial terms. */
    if (!cache) {
        /* initial capacity at least 8, but grown to n+1 if larger */
        cap = 8;
        if (n + 1 > cap) cap = n + 1;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */

        /* initialise first 8 terms (the rest, if any, are zero for now) */
        static const seq_t init[8] = {0, 1, 4, 7, 11, 16, 22, 29};
        for (idx_t i = 0; i < 8 && i < cap; ++i)
            cache[i] = init[i];
        len = 8;                       /* 8 terms known, may be truncated */
    }

    /* Grow the cache if the requested index is outside the allocated space. */
    if ((idx_t)(n + 1) > cap) {
        idx_t newCap = cap * 2;
        if (newCap < (idx_t)(n + 1))
            newCap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)newCap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        cap = newCap;
    }

    /* If the term has already been computed, just return it. */
    if (n < len)
        return cache[n];

    /* Compute the missing terms up to the requested index. */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence is valid for i >= 8 because len >= 8 after init */
        cache[i] = 2 * cache[i - 1] - cache[i - 2]
                  + cache[i - 6] - 2 * cache[i - 7] + cache[i - 8];
    }
    len = n + 1;
    return cache[n];
}
