/*
 * A290903  p-INVERT of the positive integers, where p(S) = 1 - 5*S.
 * https://oeis.org/A290903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5, a(1) = 35, a(n) = 7*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static cache so that calls in arbitrary order are O(1)
 * after the initial expansion. It works with both C11 and C++17.
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* Ensure the cache covers the requested index */
    if (n + 1 > cache_sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) /* allocation failed – return 0 as a safe fallback */
            return 0;
        cache = tmp;

        /* Initialize the first terms if this is the first allocation
         * or if the cache has become empty due to reallocation. */
        if (cache_sz == 0) {
            cache[0] = (seq_t)5;
        }
        if (new_sz > 1 && cache_sz < 2) {
            cache[1] = (seq_t)35;
        }

        /* Compute missing terms using the recurrence. */
        idx_t start = (cache_sz > 1) ? cache_sz : 2;
        for (idx_t i = start; i < new_sz; ++i) {
            cache[i] = (seq_t)(7 * cache[i-1] - cache[i-2]);
        }

        cache_sz = new_sz;
    }

    return cache[n];
}
