/*
 * A249569  a(1) = a(2) = 1; for n > 2, a(n) = a(n-a(n-1)) + a(n-a(a(n-2))).
 * https://oeis.org/A249569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1; a(n)=a(n-a(n-1))+a(n-a(a(n-2))) for n>2
 * Range:     u8 n <= 378, u16 n <= 9068, u32 n <= 9068, u64 n <= 9068,
 *            u128 n <= 9068, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation: indices 1..cap-1 are valid */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated size (cache[0] unused) */
    static size_t filled  = 0;   /* highest index computed so far */

    if (n <= 2)                /* base values */
        return (seq_t)1;

    /* ensure we can index up to n */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();    /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first two terms if not yet stored */
    if (filled == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        filled   = 2;
    }

    /* compute missing terms up to n */
    for (size_t i = filled + 1; i <= (size_t)n; ++i) {
        /* indices required by the recurrence */
        seq_t prev   = cache[i - 1];
        seq_t inner  = cache[i - 2];

        idx_t idx1 = (idx_t)i - (idx_t)prev;          /* n - a(n-1)   */
        idx_t inner_idx = (idx_t)inner;               /* a(n-2)      */
        idx_t idx2 = (idx_t)i - (idx_t)cache[(size_t)inner_idx]; /* n - a(a(n-2)) */

        /* store the term */
        cache[i] = cache[(size_t)idx1] + cache[(size_t)idx2];
    }
    filled = (size_t)n;

    return cache[(size_t)n];
}
