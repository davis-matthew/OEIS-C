/*
 * A398727  Lower (1/2, 3) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398727: lower (1/2,3) midsequence of triangular and tetrahedral numbers.
   Recurrence (OEIS formula):
       a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) - a(n-6)
   Initial values: a(0)=0, a(1)=3, a(2)=16, a(3)=45, a(4)=95, a(5)=172.
*/

#include <stdlib.h>   /* malloc, free, realloc, abort */

/* Return the n-th term of A398727. The function is memoised for
   arbitrary non‑decreasing n and works for any seq_t that supports
   the basic arithmetic operators. */
seq_t a(idx_t n)
{
    /* Fixed initial segment of the sequence */
    static const seq_t init[6] = {(seq_t)0, (seq_t)3, (seq_t)16,
                                  (seq_t)45, (seq_t)95, (seq_t)172};

    /* Return a directly for the first six indices */
    if (n < 6)
        return init[(size_t)n];

    /* Static memoisation state */
    static seq_t *cache = NULL;        /* array holding computed values   */
    static size_t cache_len = 0;       /* number of values currently stored (>=6 after init) */
    static size_t cache_cap = 0;       /* allocated capacity of the array   */

    /* Initialise memoisation on first call */
    if (cache_len == 0) {
        cache_cap = 6;
        cache_len = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }

    /* Ensure sufficient capacity for the requested index */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t val = (seq_t)4 * cache[i - 1]
                  - (seq_t)7 * cache[i - 2]
                  + (seq_t)8 * cache[i - 3]
                  - (seq_t)7 * cache[i - 4]
                  + (seq_t)4 * cache[i - 5]
                  - cache[i - 6];
        cache[i] = val;
    }
    cache_len = needed;

    return cache[(size_t)n];
}
