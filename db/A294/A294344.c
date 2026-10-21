/*
 * A294344  a(n) = ((-9*n + 82)*10^n - 1)/81.
 * https://oeis.org/A294344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 18, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A294344:
   a(n) = ((-9*n + 82)*10^n - 1)/81
   Recurrence:  a(0)=1, a(1)=9, a(2)=79,
                a(n)=21*a(n-1)-120*a(n-2)+100*a(n-3)  for n>2
   The function caches computed values so each index is evaluated at most once. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;             /* allocated capacity */
    static idx_t  last = (idx_t)-1;    /* highest index computed; sentinel */

    /* extend cache if needed for index n */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL)
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    size_t start = (last == (idx_t)-1) ? 0 : ((size_t)last + 1);
    for (size_t i = start; i <= (size_t)n; ++i) {
        idx_t ii = (idx_t)i;      /* cast for recurrence indexing */
        if (ii == 0)
            cache[0] = (seq_t)1;
        else if (ii == 1)
            cache[1] = (seq_t)9;
        else if (ii == 2)
            cache[2] = (seq_t)79;
        else
            cache[ii] = (seq_t)(21 * cache[ii-1] - 120 * cache[ii-2] + 100 * cache[ii-3]);
    }
    last = n;

    return cache[(size_t)n];
}
