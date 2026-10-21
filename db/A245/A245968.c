/*
 * A245968  The edge independence number of the Lucas cube Lambda(n).
 * https://oeis.org/A245968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((L(n)-1)/2) where L(n) are Lucas numbers (L(0)=2,
 *            L(1)=1, L(n)=L(n-1)+L(n-2) for n≥2). The sequence also satisfies
 *            the linear recurrence a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5)
 *            for n≥5 with initial values a(0)=0, a(1)=0, a(2)=1, a(3)=1,
 *            a(4)=3
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Initial values for the first five terms */
    static const seq_t init[5] = {
        (seq_t)0, /* a(0) */
        (seq_t)0, /* a(1) */
        (seq_t)1, /* a(2) */
        (seq_t)1, /* a(3) */
        (seq_t)3  /* a(4) */
    };

    /* Direct return for small n */
    if (n < 5)
        return init[n];

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;
    static idx_t  cache_next = 0; /* index of first uninitialised entry */

    /* Ensure cache is large enough to store values up to n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failure – return a default */
            return init[4];
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first five entries if they have not yet been set */
    if (cache_next < 5) {
        /* Copy only the part that is still unset */
        idx_t i;
        for (i = cache_next; i < 5; ++i)
            cache[i] = init[i];
        cache_next = 5;
    }

    /* Compute forward from cache_next to n using the recurrence */
    for (idx_t i = cache_next; i <= n; ++i) {
        /* For n ≥ 5 the recurrence is valid */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3]
                  - cache[i-4] - cache[i-5];
        cache[i] = sum;
    }

    /* Update cache_next to indicate all entries up to n are computed */
    if (cache_next <= n)
        cache_next = n + 1;

    return cache[n];
}
