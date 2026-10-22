/*
 * A349048  G.f. A(x) satisfies: A(x) = 1 / (1 - x + x^4 * A(x)).
 * https://oeis.org/A349048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 45, i64 n <= 81, i128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=a(1)=a(2)=a(3)=1, a(4)=0;
 *            a(n)=a(n-1)-Σ_{k=0}^{n-4} a(k)*a(n-4-k)   for n≥5
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t max_computed = 0;
    static int initialized = 0;

    /* Initialization: store the first five terms */
    if (!initialized) {
        cache_capacity = 8;                /* initial capacity */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* out‑of‑memory guard */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;

        max_computed = 4;
        initialized = 1;
    }

    /* Return immediately if the value is already cached */
    if (n <= max_computed) return cache[n];

    /* Grow the cache so that index n is reachable */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity << 1 : 8;
        while (new_cap <= n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory guard */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms up to n, using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* i≥5 here, so i-4 is safe */
        for (idx_t k = 0; k <= i - 4; ++k) {
            sum += cache[k] * cache[i - 4 - k];
        }
        cache[i] = cache[i - 1] - sum;
    }

    max_computed = n;
    return cache[n];
}
