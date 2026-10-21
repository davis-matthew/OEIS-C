/*
 * A243094  Cardinality of the Weyl alternation set corresponding to the
 *          zero-weight in the representation of the Lie algebra sp(2n) whose
 *          highest weight is the second fundamental weight.
 * https://oeis.org/A243094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4) for n >= 5; a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=8, a(4)=19
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_computed = (idx_t)-1;

    /* Initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 5;                                 /* indices 0..4 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* Allocation failure unlikely for small n */

        cache[0] = 1;  /* a(0) */
        cache[1] = 2;  /* a(1) */
        cache[2] = 5;  /* a(2) */
        cache[3] = 8;  /* a(3) */
        cache[4] = 19; /* a(4) */

        max_computed = 4;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow the cache so it can hold almost up to a(n) */
    if ((size_t)(n) + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* Allocation failure unlikely for small n */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the sequence from the first missing index up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence holds for i >= 5.  For i == 0..4 the cache already contains
           the correct initial values, so this loop will never execute for i < 5. */
        cache[i] = cache[i - 1] + cache[i - 2] + 3 * cache[i - 3] + cache[i - 4];
    }

    max_computed = n;
    return cache[n];
}
