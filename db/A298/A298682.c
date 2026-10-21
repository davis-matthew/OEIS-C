/*
 * A298682  Start with the triangle with 4 markings of the Shield tiling and
 *          recursively apply the substitution rule. a(n) is the number of
 *          triangles with 4 markings after n iterations.
 * https://oeis.org/A298682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) - 9*a(n-3) + 2*a(n-4) for n>3 Initial
 *            terms: a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t  cap    = 0;      /* allocated capacity of cache */
    static size_t  len    = 0;      /* number of computed terms in cache */

    /* Initialize the cache with the first four terms once */
    if (len == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;           /* OOM – safe fallback */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 8;
        len    = 4;
    }

    /* If the requested term is already computed, return it */
    if ((idx_t)n < len)
        return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        /* double until we have enough space for n */
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* OOM – safe fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms from len up to n inclusively */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* recurrence: 3*a_{i-1} + 5*a_{i-2} - 9*a_{i-3} + 2*a_{i-4} */
        seq_t term = 3 * cache[i-1]
                   + 5 * cache[i-2]
                   - 9 * cache[i-3]
                   + 2 * cache[i-4];
        cache[i] = term;
    }
    len = (size_t)(n + 1); /* all terms up to n are now computed */

    return cache[n];
}
