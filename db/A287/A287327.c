/*
 * A287327  Number of independent vertex sets (and vertex covers) in the
 *          2n-crossed prism graph.
 * https://oeis.org/A287327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=7, a(n)=7*a(n-1)-7*a(n-2) for n>1 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static idx_t last   = 0;      /* highest index that has been computed */
    static size_t cap   = 0;      /* allocated capacity */

    /* initialise cache with the two base cases */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* guard – should never happen */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)7;
        last = 1;
    }

    /* already computed? */
    if (n <= last)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* guard – should never happen */
        cache = tmp;
        cap   = needed;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)7 * cache[(size_t)(i - 1)] -
            (seq_t)7 * cache[(size_t)(i - 2)];
    }
    last = n;
    return cache[(size_t)n];
}
