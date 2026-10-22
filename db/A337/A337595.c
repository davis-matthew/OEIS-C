/*
 * A337595  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k *
 *          5^(k-1) * a(n-k).
 * https://oeis.org/A337595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=1..n} binom(n,k)^2 * k * 5^(k-1) * a(n-k)
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Public function, external linkage */

#include <stdlib.h>
#include <stddef.h>

static seq_t *alloc_seq_arr(size_t cap) {
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return malloc(cap * sizeof(seq_t));
#endif
}

static seq_t *realloc_seq_arr(seq_t *old, size_t old_cap, size_t new_cap) {
#if defined(__cplusplus)
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) new_arr[i] = old[i];
    delete[] old;
    return new_arr;
#else
    return realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t computed_cap = 0;   /* number of entries *actually* filled */

    /* Allocate initial cache if needed */
    if (cache_cap == 0) {
        cache = alloc_seq_arr(1);
        if (!cache) return 0;      /* allocation failure, unlikely in tests */
        cache[0] = 1;             /* a(0) = 1 */
        cache_cap = 1;
        computed_cap = 1;
    }

    /* Ensure the cache is large enough for indices up to n */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = realloc_seq_arr(cache, cache_cap, new_cap);
        if (!tmp) return 0;       /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_cap; i <= n; ++i) {
        if (i == 0) continue;    /* already set */

        seq_t sum = 0;
        seq_t bin = 1;           /* binom(i,0) */
        seq_t pow5 = 1;          /* 5^(k-1) for k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* binom(i,k) = binom(i,k-1) * (i - k + 1) / k */
            bin = (bin * (seq_t)(i - k + 1)) / (seq_t)k;

            seq_t bin_sq = bin * bin;
            seq_t term = bin_sq * (seq_t)k;
            term = term * pow5;
            term = term * cache[i - k];
            sum += term;

            pow5 = pow5 * (seq_t)5;   /* prepare 5^(k) for next k */
        }
        cache[i] = sum / (seq_t)i;
    }

    computed_cap = n + 1;
    return cache[n];
}
