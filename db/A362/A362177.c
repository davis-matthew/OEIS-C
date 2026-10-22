/*
 * A362177  Expansion of e.g.f. exp(x * (1-3*x)).
 * https://oeis.org/A362177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(n)=a(n-1)-6*(n-1)*a(n-2) for n>1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 22, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms */
static seq_t *cache = NULL;     /* pointer to array of a(0)..a(n) */
static idx_t cache_cap = 0;     /* allocated capacity (elements)    */
static idx_t cache_len = 0;     /* number of terms already computed */

/* Ensure capacity for at least (n+1) elements and initialise a(0), a(1) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise with at least two slots */
        cache_cap = n + 1;
        if (cache_cap < 2) cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* fatal error if malloc fails */

        cache[0] = (seq_t)1;                 /* a(0) */
        cache[1] = (seq_t)1;                 /* a(1) */
        cache_len = 2;                       /* two terms stored now */
    } else if (cache_cap <= n) {
        /* grow the buffer (double strategy) */
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* fatal error if realloc fails */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute terms up to and including n (if not already done) */
static void compute_to(idx_t n)
{
    if (n + 1 <= cache_len) return;   /* already have all needed */

    ensure_cache(n);                  /* guarantees space for a(n) */

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;       /* a(0) handled by ensure_cache */
        } else if (i == 1) {
            cache[i] = (seq_t)1;       /* a(1) handled by ensure_cache */
        } else {
            seq_t coef = (seq_t)(i - 1) * 6;   /* 6*(n-1) */
            cache[i] = cache[i - 1] - coef * cache[i - 2];
        }
    }
    cache_len = n + 1;                 /* now a(0)..a(n) stored */
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
