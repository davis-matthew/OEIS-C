/*
 * A285760  a(n) = a(n - 2 - a(n - 1)) + a(n - 2 - a(n - 4)), with a(1) = 1,
 *          a(2) = 1, a(3) = 2, a(4) = 2.
 * https://oeis.org/A285760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 517, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n - 2 - a(n - 1)) + a(n - 2 - a(n - 4)),
   a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 2.
   The function uses a static cache so that any order of calls is
   handled in O(n) time for the largest n seen so far. */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i) for i ≥ 1; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* array capacity (number of elements) */
    static idx_t cache_last = 0;          /* largest index already computed */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 8;                    /* start with capacity for 8 entries */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed: return 0 */

        cache[0]   = 0;                   /* unused */
        cache[1]   = (seq_t)1;            /* a(1) */
        cache[2]   = (seq_t)1;            /* a(2) */
        cache[3]   = (seq_t)2;            /* a(3) */
        cache[4]   = (seq_t)2;            /* a(4) */
        cache_last = 4;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    while (cache_last < n) {
        idx_t i = cache_last + 1;                 /* next index to compute */

        /* compute indices for the recurrence */
        idx_t idx1 = i - (idx_t)2 - (idx_t)cache[i - 1];
        idx_t idx2 = i - (idx_t)2 - (idx_t)cache[i - 4];

        /* guarantee indices are within bounds (they should be) */
        if (idx1 < (idx_t)1) idx1 = 1;
        if (idx2 < (idx_t)1) idx2 = 1;

        /* obtain terms from the cache */
        seq_t term1 = cache[(size_t)idx1];
        seq_t term2 = cache[(size_t)idx2];

        cache[i] = term1 + term2;                 /* compute a(i) */
        cache_last = i;
    }

    return cache[n];
}
