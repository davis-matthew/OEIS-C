/*
 * A263418  a(n) is the total number of pentagrams in a variant of pentagram
 *          fractal after n iterations.
 * https://oeis.org/A263418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-2*a(n-2)-a(n-3)+a(n-4); with initial values
 *            a(0)=1, a(1)=6, a(2)=21, a(3)=51
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap  = 0;       /* allocated capacity */
    static idx_t  last = (idx_t)-1; /* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with space for 4 terms (0..3) */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out‑of‑memory – never expected */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)51;
        last = 3;
    }

    /* if requested index is already cached, return it. */
    if (n <= last)
        return cache[n];

    /* grow the cache to accommodate indices up to n */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;                /* fast growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* out‑of‑memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute new terms using the recurrence */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t t1 = cache[k-1] * (seq_t)3;
        t1 += cache[k-4];
        seq_t t2 = cache[k-2] * (seq_t)2;
        t2 += cache[k-3];
        cache[k] = t1 - t2;
    }
    last = n;
    return cache[n];
}
