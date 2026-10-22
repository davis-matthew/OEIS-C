/*
 * A386264  G.f. A(x) satisfies A(x) = 1/( (1-x)^3 * (1 - x*A(x) -
 *          2*x^2*A'(x)) ).
 * https://oeis.org/A386264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+2,2) + n * Sum_{k=0..n-1} a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A386264 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* highest pre‑computed index */

    /* Initialise cache for a(0) on first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_len = 0;
    }

    /* Extend cache if needed */
    if (n > cache_len) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                sum += cache[k] * cache[i - 1 - k];
            }
            seq_t term = (seq_t)(i + 2) * (seq_t)(i + 1) / 2;
            term += (seq_t)i * sum;
            cache[i] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
