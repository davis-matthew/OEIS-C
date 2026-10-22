/*
 * A364763  G.f. satisfies A(x) = 1 / (1 + x*(1 + x*A(x))^5).
 * https://oeis.org/A364763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} (-1)^k * binomial(n+1,k) *
 *            binomial(5*k,n-k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Multiplicative binomial: C(a,b) */
static seq_t binom(idx_t a, idx_t b)
{
    if (b > a)
        return (seq_t)0;
    if (b > a - b)
        b = a - b;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= b; ++i) {
        res = res * (seq_t)(a - b + i) / (seq_t)i;
    }
    return res;
}

/* Ensure cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Lazy allocation */
    if (cache == NULL) {
        cache_cap  = 16;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* cache[0] = 1 */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n < cache_size)
        return;

    /* Grow capacity if needed */
    if (cache_cap < n + (idx_t)1) {
        idx_t new_cap = cache_cap ? cache_cap * (idx_t)2 : (idx_t)8;
        while (new_cap < n + (idx_t)1)
            new_cap *= (idx_t)2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom_n1k = (seq_t)1;  /* binom(i+1, 0) */

        for (idx_t k = (idx_t)0; k <= i; ++k) {
            seq_t binom_5k_nk  = binom((idx_t)5 * k, i - k);
            seq_t term = binom_n1k * binom_5k_nk;

            if (k & (idx_t)1)
                sum -= term;
            else
                sum += term;

            if (k < i) {
                /* binom(i+1, k+1) = binom(i+1, k) * (i+1-k) / (k+1) */
                binom_n1k = binom_n1k * (seq_t)(i + (idx_t)1 - k) / (seq_t)(k + (idx_t)1);
            }
        }

        cache[i] = sum / (seq_t)(i + (idx_t)1);
    }

    cache_size = n + (idx_t)1;
}

/* Public function */
seq_t a(idx_t n)
{
    /* Static cache inside the function ensures memoisation across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Lazy allocation */
    if (cache == NULL) {
        cache_cap  = 16;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n < cache_size)
        return cache[n];

    /* Grow capacity if needed */
    if (cache_cap < n + (idx_t)1) {
        idx_t new_cap = cache_cap ? cache_cap * (idx_t)2 : (idx_t)8;
        while (new_cap < n + (idx_t)1)
            new_cap *= (idx_t)2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom_n1k = (seq_t)1;  /* binom(i+1, 0) */

        for (idx_t k = (idx_t)0; k <= i; ++k) {
            seq_t binom_5k_nk  = binom((idx_t)5 * k, i - k);
            seq_t term = binom_n1k * binom_5k_nk;

            if (k & (idx_t)1)
                sum -= term;
            else
                sum += term;

            if (k < i) {
                /* binom(i+1, k+1) = binom(i+1, k) * (i+1-k) / (k+1) */
                binom_n1k = binom_n1k * (seq_t)(i + (idx_t)1 - k) / (seq_t)(k + (idx_t)1);
            }
        }

        cache[i] = sum / (seq_t)(i + (idx_t)1);
    }

    cache_size = n + (idx_t)1;
    return cache[n];
}
