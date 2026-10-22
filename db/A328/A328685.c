/*
 * A328685  Row sums of A309038.
 * https://oeis.org/A328685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4) - 6*a(n-5) + 3*a(n-6) -
 *            3*a(n-8) + 6*a(n-9) - 3*a(n-10) + a(n-12) - 2*a(n-13) + a(n-14)
 *            for n > 18 Initial values (n = 0 … 18): 0, 4, 28, 120, 320, 716,
 *            1380, 2464, 3984, 6196, 9124, 13128, 18048, 24476, 32244, 42096,
 *            53440, 67460, 83604
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Small table of initial values */
    static const seq_t init[19] = {
        (seq_t)0,  (seq_t)4,  (seq_t)28,  (seq_t)120, (seq_t)320,
        (seq_t)716, (seq_t)1380, (seq_t)2464, (seq_t)3984, (seq_t)6196,
        (seq_t)9124, (seq_t)13128, (seq_t)18048, (seq_t)24476, (seq_t)32244,
        (seq_t)42096, (seq_t)53440, (seq_t)67460, (seq_t)83604
    };

    /* Return known values directly */
    if (n <= 18) return init[n];

    /* Static cache to store computed values up to the largest n requested so far */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 18;          /* highest index that is computed in cache */

    /* Allocate or grow cache to accommodate index n */
    if (cache == NULL) {
        size_t cap = (size_t)n + 1;
        if (cap < 19) cap = 19;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* initialize known entries */
        for (size_t i = 0; i < 19; ++i) cache[i] = init[i];
        cache_size = cap;
    } else if ((idx_t)n >= cache_size) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute missing values in the cache */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i-1]
                 - cache[i-2]
                 + (seq_t)3 * cache[i-4]
                 - (seq_t)6 * cache[i-5]
                 + (seq_t)3 * cache[i-6]
                 - (seq_t)3 * cache[i-8]
                 + (seq_t)6 * cache[i-9]
                 - (seq_t)3 * cache[i-10]
                 + cache[i-12]
                 - (seq_t)2 * cache[i-13]
                 + cache[i-14];
    }
    computed = n;

    return cache[n];
}
