/*
 * A298355  a(n) = a(n-1) + a(n-2) + 2 a([(n-1)/2]), where a(0) = 1, a(1) = 2,
 *          a(2) = 3.
 * https://oeis.org/A298355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(floor((n-1)/2)); a(0) = 1, a(1) =
 *            2, a(2) = 3
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute sequence A298355 */
seq_t a(idx_t n) {
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated capacity */
    static idx_t computed = 0;    /* highest index computed */

    /* Initialise cache on first call or when nothing computed yet */
    if (!cache) {
        /* start with capacity 4 to hold indices 0..3 */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */

        /* base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        computed = 2;
    }

    /* extend cache if requested index exceeds current capacity */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* If requested term already computed, return it */
    if (n <= computed) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t mid = (i - 1) / 2;
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)2 * cache[mid];
    }
    computed = n;

    return cache[n];
}
