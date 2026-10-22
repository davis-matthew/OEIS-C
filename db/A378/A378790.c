/*
 * A378790  Lower matching number of the n X n king graph.
 * https://oeis.org/A378790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) (n≥8)
 *            with initial values: a(1)=0, a(2)=2, a(3)=3, a(4)=6, a(5)=8,
 *            a(6)=14, a(7)=17
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of values, 1‑based indexing. */
    enum { MAX_CACHE = 10001 };               /* enough for n≈10000   */
    static seq_t cache[MAX_CACHE] = {0};
    static idx_t cache_size = 0;
    static const seq_t init[7] = {
        (seq_t)0, (seq_t)2, (seq_t)3,
        (seq_t)6, (seq_t)8, (seq_t)14,
        (seq_t)17
    };

    /* Lazily initialize cache with the first 7 values. */
    if (cache_size == 0) {
        for (idx_t i = 1; i <= 7; ++i) {
            cache[i] = init[i - 1];
        }
        cache_size = 7;
    }

    if (n <= cache_size) {
        return cache[n];
    }

    if (n >= MAX_CACHE) {
        /* n too large for this fixed cache size – allocate dynamic array. */
        static seq_t *dyncache = NULL;
        static idx_t dynsize = 0;
        if (dyncache == NULL) {
            dyncache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
            if (!dyncache) abort();          /* allocation failed */
            for (idx_t i = 1; i <= 7; ++i) {
                dyncache[i] = init[i - 1];
            }
            dynsize = 7;
        } else if (n > dynsize) {
            seq_t *tmp = (seq_t*)realloc(dyncache, (n + 1) * sizeof(seq_t));
            if (!tmp) abort();
            dyncache = tmp;
            dynsize = n;
        }

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t pos = dyncache[i-1] + dyncache[i-2] + dyncache[i-4] + dyncache[i-7];
            seq_t neg = dyncache[i-3] + dyncache[i-5] + dyncache[i-6];
            dyncache[i] = pos - neg;
        }
        cache_size = n;
        return dyncache[n];
    }

    /* Compute values up to n using the static cache. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = cache[i-1] + cache[i-2] + cache[i-4] + cache[i-7];
        seq_t neg = cache[i-3] + cache[i-5] + cache[i-6];
        cache[i] = pos - neg;
    }
    cache_size = n;
    return cache[n];
}
