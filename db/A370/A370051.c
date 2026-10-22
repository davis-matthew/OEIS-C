/*
 * A370051  Expansion of (1-5*x+6*x^2-x^3)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A370051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5) *
 *            Initial values: a(0)=1, a(1)=4, a(2)=14, a(3)=48, a(4)=165 * The
 *            implementation uses dynamic caching and works for any seq_t / *
 *            idx_t that support basic arithmetic and standard allocation
 *            functions
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial five terms (static to avoid re‑allocation on each call) */
static const seq_t init_vals[5] = {
    (seq_t)1,
    (seq_t)4,
    (seq_t)14,
    (seq_t)48,
    (seq_t)165
};

static seq_t *cache = NULL;
static idx_t  cache_len = 0; /* number of terms already stored (>=5 if cache is in use) */
static idx_t  cache_cap = 0; /* current allocated capacity */

/* Ensure the cache can hold indices up to 'need' (inclusive) */
static void ensure_capacity(idx_t need)
{
    if (need + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < need + 1) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory – terminate */
        cache       = tmp;
        cache_cap   = new_cap;
    }
}

/* Allocate the cache and store the first five terms, if not already done */
static void init_cache(idx_t min_cap)
{
    if (cache == NULL) {
        cache_cap = (min_cap > 5 ? min_cap : 5);
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 5; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 5;
    }
}

/* Generate terms up to index n (inclusive); the cache must be initialized */
static void ensure_terms(idx_t n)
{
    if (cache_len == 0) {
        init_cache(n + 1);
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_capacity(i);
        seq_t v1 = 9 * cache[i-1];
        seq_t v2 = 35 * cache[i-3];
        seq_t v3 = cache[i-5];
        seq_t v4 = 28 * cache[i-2];
        seq_t v5 = 15 * cache[i-4];
        seq_t val = v1 + v2 + v3;
        val -= v4;
        val -= v5;
        cache[i] = val;
    }
    cache_len = n + 1;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n < 5) {
        return init_vals[(size_t)n];
    }
    ensure_terms(n);
    return cache[n];
}
