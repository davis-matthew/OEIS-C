/*
 * A283649  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 705", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283649
   a(0) = 1
   a(1) = 0
   a(2) = 111
   a(3) = 1011
   a(4) = 111
   a(5) = 101111
   For n>5 recurrence: a(n) = a(n-1) + 100*a(n-2) - 100*a(n-3)   (n>5) */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Base values for the first six terms */
    static const seq_t base[6] = {1, 0, 111, 1011, 111, 101111};

    /* For the first six indices return immediately */
    if (n < 6) {
        return base[n];
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of computed entries (max index + 1) */

    /* Ensure the cache is allocated and holds the first six terms */
    if (!cache) {
        /* Allocate at least enough space for the requested index */
        size_t cap = n + 1;
        if (cap < 6) cap = 6;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = base[i];
        }
        cache_len = 6;        /* indices 0..5 are known */
    }

    /* Grow the cache if the current maximum index is insufficient */
    if (n + 1 > (size_t)cache_len) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        /* keep cache_len unchanged (still number of known entries) */
    }

    /* Compute any missing terms up to index n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prod2 = cache[i - 2] * (seq_t)100;
            seq_t prod3 = cache[i - 3] * (seq_t)100;
            cache[i] = cache[i - 1] + prod2 - prod3;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
