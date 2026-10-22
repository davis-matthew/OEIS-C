/*
 * A397588  G.f. A(x) satisfies A(x) = x + (x*A(x)^2)'.
 * https://oeis.org/A397588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Sum_{k=1}^{n-1} a(k) * a(n-k), a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to grow the cache as needed */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap) {
    if (*cache == NULL) {
        *cap = (size_t)n + 1;
        *cache = (seq_t*)malloc((*cap) * sizeof(seq_t));
    } else if (*cap <= (size_t)n) {
        size_t new_cap = *cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        *cache = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = 0;   /* highest index computed (>=1) */

    /* Request computation up to n */
    if (n > (idx_t)max_computed) {
        ensure_cache(n, &cache, &capacity);

        /* Initialize base case if first time */
        if (max_computed == 0) {
            cache[1] = 1;
            max_computed = 1;
        }

        /* Compute missing terms in increasing order */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t m = (i - 1) / 2;          /* floor((i-1)/2) */
            for (idx_t k = 1; k <= m; ++k) {
                seq_t prod = cache[k] * cache[i - k];
                sum += prod + prod;         /* double count symmetric terms */
            }
            if ((i & 1) == 0) {            /* i even -> middle term once */
                seq_t prod = cache[i / 2];
                prod = prod * prod;
                sum += prod;
            }
            /* a(i) = (i+1) * sum */
            seq_t factor = (seq_t)i + (seq_t)1;
            cache[i] = factor * sum;
        }
        max_computed = n;
    }

    return cache[n];
}
