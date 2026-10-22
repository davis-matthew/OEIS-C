/*
 * A368452  Expansion of e.g.f. exp(-x) / (1 + log(1 - 3*x)/3).
 * https://oeis.org/A368452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Σ_{k=1..n} 3^(k-1)*(k-1)!*binomial(n,k)*a(n-k),
 *            a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* Allocate or grow cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* small safeguard: out‑of‑memory */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[0] = 1;                  /* a(0) = 1 */
    }
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;      /* small safeguard: out‑of‑memory */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already computed */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t comb = (seq_t)i;          /* C(i,1) */
        seq_t b = 1;                    /* 3^(k-1)*(k-1)! for k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = b * comb * cache[i - k];
            sum += term;

            if (k < i) {
                /* Next k: multiply b by 3*k     (since b_{k+1} = b_k * 3 * k) */
                b = b * 3 * (seq_t)k;
                /* Update binomial coefficient: C(i,k+1) = C(i,k)*(i-k)/(k+1) */
                comb = comb * ((seq_t)i - (seq_t)k) / ((seq_t)(k + 1));
            }
        }

        /* Add (-1)^i term */
        if ((i & 1) == 0)       /* even i */
            sum += 1;
        else                    /* odd i: subtract 1   */
            sum -= 1;

        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
