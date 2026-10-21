/*
 * A263043  a(n)=n for n = 1..5; for n>=6, a(n) = a(n-5)*a(n-3)*a(n-1) -
 *          a(n-4)*a(n-2).
 * https://oeis.org/A263043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5)*a(n-3)*a(n-1) - a(n-4)*a(n-2) initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Local static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    if (n == 0) return 0;            /* Undefined, but harmless */
    if (cache == NULL) {              /* First call: allocate and init */
        cache_cap = (n > 5 ? n : 5);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)5;
        cache_size = 5;
    }

    if (n <= cache_size) return cache[n - 1];

    if (n > cache_cap) {              /* Expand cache if needed */
        size_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* indices are 1-based; convert to 0-based for array access */
        seq_t t1 = cache[k - 6] * cache[k - 4] * cache[k - 2]; /* a(k-5)*a(k-3)*a(k-1) */
        seq_t t2 = cache[k - 5] * cache[k - 3];                /* a(k-4)*a(k-2) */
        cache[k - 1] = t1 - t2;
    }
    cache_size = n;
    return cache[n - 1];
}
