/*
 * A291025  p-INVERT of the positive integers, where p(S) = 1 - 3*S + S^2.
 * https://oeis.org/A291025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 13*a(n-2) + 7*a(n-3) - a(n-4); a(0)=3,
 *            a(1)=14, a(2)=62, a(3)=273
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A291025.
   Uses memoisation to avoid recomputation on repeated calls. */
seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[4] = {3, 14, 62, 273};

    /* Cache variables */
    static seq_t *cache = NULL;     /* dynamically grown array */
    static idx_t last = (idx_t)-1;  /* largest n for which cache[n] is valid */
    static size_t cap = 0;          /* current allocated capacity (elements) */

    /* For n < 4, return the hard‑coded initial values directly */
    if (n < 4)
        return init[n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cap = 4;  /* at least 4 elements are needed to store the base cases */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;  /* allocation failure – unlikely in test harness */

        for (idx_t i = 0; i < 4; ++i)
            cache[i] = init[i];

        last = 3;  /* highest computed index */
    }

    /* If the requested n is already computed, return it immediately */
    if (n <= last)
        return cache[n];

    /* Grow the cache if necessary */
    while (cap <= n) {
        size_t new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;  /* allocation failure – unlikely in test harness */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms from (last+1) up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = cache[i-1] * 7   - cache[i-2] * 13
                    + cache[i-3] * 7 - cache[i-4];
        cache[i] = val;
    }

    last = n;           /* update the highest computed index */
    return cache[n];
}
