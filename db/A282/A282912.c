/*
 * A282912  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 526", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5) (n>5) a(0)=1, a(1)=11,
 *            a(2)=111, a(3)=1011, a(4)=10011, a(5)=110111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage function for OEIS sequence A282912 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* pointer to array of seq_t */
    static size_t cache_cap = 0;          /* allocated capacity (>=n+1) */
    static idx_t last_computed = (idx_t)-1;  /* highest index computed */

    /* initialise cache and base values on first call */
    if (cache == NULL) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* allocation failed */
        /* base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)111;
        cache[3] = (seq_t)1011;
        cache[4] = (seq_t)10011;
        cache[5] = (seq_t)110111;
        last_computed = 5;
    }

    /* if requested index is already computed, return immediately */
    if (n <= last_computed)
        return cache[n];

    /* ensure cache has enough capacity to hold up to index n */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 6;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 10 * cache[i-1]   /* 10*a(i-1) */
                 + cache[i-4]        /* + a(i-4) */
                 - 10 * cache[i-5];  /* - 10*a(i-5) */
    }
    last_computed = n;

    return cache[n];
}
