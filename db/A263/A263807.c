/*
 * A263807  Total number of ON (black) cells after n iterations of the "Rule
 *          157" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A263807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=6, a(3)=11,
   a(n)=2*a(n-1)-2*a(n-3)+a(n-4)  (n>3)  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;      /* allocated capacity (in elements) */
    static idx_t  size  = 0;      /* number of elements actually filled */

    /* Initialise static cache on first call */
    if (cache == NULL) {
        /* Start with room for at least 4 terms */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Grow the cache until it can hold index n */
    while (cap <= n) {
        idx_t newcap = cap ? (cap << 1) : 4;
        if (newcap <= cap) { /* overflow protection */
            newcap = cap + 1;
        }
        cap = newcap;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Initialise the first four terms if not already done */
    if (size < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)11;
        size = 4;
    }

    /* Compute values up to n (if needed) */
    for (idx_t i = size; i <= n; ++i) {
        seq_t term;
        /* Compute a(i) = 2*a(i-1) - 2*a(i-3) + a(i-4) */
        term = cache[i-1] * ((seq_t)2);       /* 2*a(i-1)   */
        term -= cache[i-3] * ((seq_t)2);      /* -2*a(i-3)  */
        term += cache[i-4];                   /* + a(i-4)   */
        cache[i] = term;
    }
    if (size <= n) {
        size = n + 1;
    }

    return cache[n];
}
