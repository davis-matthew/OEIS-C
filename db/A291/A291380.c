/*
 * A291380  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S^5.
 * https://oeis.org/A291380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) for n ≥ 11 follows a(n) = a(n-5) + 5*a(n-6) + 10*a(n-7) +
 *            10*a(n-8) + 5*a(n-9) + a(n-10) Initial values: a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=1, a(5)=5, a(6)=10, a(7)=10, a(8)=5,
 *            a(9)=2, a(10)=10
 * Range:     u8 n <= 16, u16 n <= 25, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* first 11 values, used for initialization and as recurrence seeds   */
    static const seq_t init[11] =
        {0, 0, 0, 0, 1, 5, 10, 10, 5, 2, 10};
    if (n < 11) return init[n];

    static seq_t *cache = NULL;      /* memoization cache */
    static size_t cache_cap = 0;     /* current allocated capacity */
    static idx_t  cache_size = 0;    /* number of terms stored */

    /* alloc cache on the first call */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1 > 11 ? n + 1 : 11);
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0; /* out‑of‑memory fallback */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        cache_size = 11;
    }

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* out‑of‑memory fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t v = cache[i - 5];
        v += cache[i - 6] * 5;
        v += cache[i - 7] * 10;
        v += cache[i - 8] * 10;
        v += cache[i - 9] * 5;
        v += cache[i -10];
        cache[i] = v;
    }
    cache_size = n + 1;
    return cache[n];
}
