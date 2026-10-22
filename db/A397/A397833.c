/*
 * A397833  Number of binary words of length n containing the consecutive word
 *          abab.
 * https://oeis.org/A397833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 4*a(n-3) - 5*a(n-4) + 2*a(n-5)
 *            Initial values: a(0)=0 , a(1)=0 , a(2)=0 , a(3)=0 , a(4)=1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms       */
    static size_t  cap   = 0;     /* current capacity in slots     */
    static idx_t   max_n = (idx_t)-1; /* largest index already defined */

    /*----- initialise cache with base values ----*/
    if (cache == NULL) {
        /* allocate at least 5 entries to hold the base values */
        size_t init_cap = 5;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cap  = init_cap;

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        max_n   = 4;
    }

    /* If the requested index is already known, just return it */
    if (n <= max_n) {
        return cache[n];
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 5*a(i-2) + 4*a(i-3) - 5*a(i-4) + 2*a(i-5) */
        seq_t term  = cache[i-1] * (seq_t)4;
        term        -= cache[i-2] * (seq_t)5;
        term        += cache[i-3] * (seq_t)4;
        term        -= cache[i-4] * (seq_t)5;
        term        += cache[i-5] * (seq_t)2;
        cache[i]    = term;
    }

    max_n = n;
    return cache[n];
}
