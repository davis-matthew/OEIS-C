/*
 * A367639  G.f. A(x) satisfies A(x) = (1 + x)^2 + x*A(x)^2 / (1 + x).
 * https://oeis.org/A367639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 6, a(3) = 16. For n >= 4: (n+1)·a(n)
 *            = (3·n-1)·a(n-1) + 2·(4·n-9)·a(n-2) + 4·(n-4)·a(n-3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoises computed terms.  It works for any
   seq_t/idx_t supplied by the build system. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* initialise first four terms if not already present */
        if (new_cap > 0) cache[0] = (seq_t)1;
        if (new_cap > 1) cache[1] = (seq_t)3;
        if (new_cap > 2) cache[2] = (seq_t)6;
        if (new_cap > 3) cache[3] = (seq_t)16;

        /* compute missing terms */
        idx_t start = cache_cap >= 4 ? cache_cap : (idx_t)4;
        for (idx_t i = start; i < new_cap; ++i) {
            seq_t term1 = ((seq_t)(3 * i - 1)) * cache[i - 1];
            seq_t term2 = ((seq_t)(2 * (4 * i - 9))) * cache[i - 2];
            seq_t term3 = ((seq_t)(4 * (i - 4))) * cache[i - 3];
            seq_t numerator = term1 + term2 + term3;
            cache[i] = numerator / ((seq_t)(i + 1));
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
