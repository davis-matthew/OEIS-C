/*
 * A380463  Partial sums of floor(n^2/13).
 * https://oeis.org/A380463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^n floor(k^2/13)
 * Range:     u8 n <= 21, u16 n <= 136, u32 n <= 5511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store partial sums.  The array is resized
       only when necessary and works for any order of calls. */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* number of elements currently cached */

    /* Initialise the cache with the first term a(0)=0 */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();        /* out of memory */
        cache[0] = 0;               /* a(0) */
    }

    /* Grow the cache if the requested index is beyond what we have
       already computed. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;

        /* Current cumulative sum is the last cached value. */
        seq_t sum = cache[cap - 1];

        /* Compute terms from 'cap' up to 'n'.  All computations are
           performed with standard integer arithmetic; the cast to
           seq_t happens after the division to avoid overflow. */
        for (idx_t i = cap; i <= n; ++i) {
            seq_t term = (seq_t)(((uint64_t)i * i) / 13ULL);
            sum += term;
            cache[(size_t)i] = sum;
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
