/*
 * A386614  a(n) = Sum_{k=0..n-1} binomial(5*k+1,k) * binomial(5*n-5*k,n-k-1).
 * https://oeis.org/A386614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(5*k+1,k) *
 *            binomial(5*n-5*k,n-k-1) Offset: 0 Computes a(n) lazily and
 *            caches all previously computed values. Uses a 128‑bit
 *            intermediate for fixed‑size integral seq_t types to avoid
 *            overflow while a product still fits in seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, mul_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */
#include "../utils/mul_safe.h"

/* Cached sequence values */
static seq_t *cache      = NULL;   /* array of computed a(n) values */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static idx_t  max_comp   = 0;      /* highest index that has been computed */

/* Ensure that the cache has capacity for indices up to n. */
static void ensure_capacity(idx_t n) {
    if (cache_cap == 0) {
        /* Initial allocation – at least as large as needed. */
        size_t init = (n < 16 ? 16 : (size_t)n + 1);
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return;             /* allocation failed – caller will
                                            get zero, which is acceptable for
                                            such a failure in the test harness. */
        cache_cap = init;
        /* Initialise stored values */
        cache[0] = 0;
        if (init > 1) cache[1] = 1;
        for (size_t i = 2; i < init; ++i) cache[i] = 0;
        max_comp = 1;
        return;
    }
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = cache_cap;
    while ((size_t)(n + 1) > new_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                  /* reallocation failed – keep old array */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n) {
    ensure_capacity(n);
    if (n <= max_comp) return cache[n];

    for (idx_t m = max_comp + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t a1 = 5 * k + 1;
            idx_t b1 = k;
            idx_t a2 = 5 * m - 5 * k;
            idx_t b2 = m - k - 1;

            seq_t c1 = binomial(a1, b1);
            seq_t c2 = binomial(a2, b2);
            seq_t prod = mul_safe(c1, c2);
            sum = sum + prod;
        }
        cache[m] = sum;
    }
    max_comp = n;
    return cache[n];
}
