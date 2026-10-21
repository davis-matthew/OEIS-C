/*
 * A281742  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 393", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281742: a(0) = 1, a(1) = 0, a(2) = 5, a(3) = 0, ...,
   a(n) = 4*a(n-2) + a(n-8) - 4*a(n-10) for n > 23.
   The first 24 terms are required as initial values. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size        */
    static size_t len   = 0;   /* number of valid entries */

    /* the 24 initial terms – these are the only hard‑coded values, as
       required by the recurrence. */
    static const seq_t init[24] = {
        1ULL,0ULL,5ULL,0ULL,21ULL,0ULL,85ULL,16ULL,
        261ULL,224ULL,1365ULL,16ULL,5381ULL,224ULL,
        21845ULL,272ULL,86021ULL,4064ULL,348245ULL,
        3600ULL,1398021ULL,20704ULL,5522773ULL,139536ULL
    };

    /* first time: allocate cache and initialise the first 24 entries */
    if (cache == NULL) {
        cap = 32;                       /* start with power of two */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */

        for (size_t i = 0; i < 24; ++i) {
            cache[i] = init[i];
        }
        len = 24;
    }

    /* if the requested index is already known, return it */
    if ((idx_t)len > n) {
        return cache[(size_t)n];
    }

    /* grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i < 24) {                   /* base terms already set */
            continue;
        }
        cache[i] = ((seq_t)4) * cache[i-2]          /* 4*a(i-2) */
                 + cache[i-8]                       /* + a(i-8) */
                 - ((seq_t)4) * cache[i-10];        /* - 4*a(i-10) */
    }
    len = needed;
    return cache[(size_t)n];
}
