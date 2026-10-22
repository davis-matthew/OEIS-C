/*
 * A385838  a(n) = 1 + Sum_{k=0..n-1} (1 + k^5) * a(k) * a(n-1-k).
 * https://oeis.org/A385838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ∑_{k=0}^{n-1} (1 + k⁵) · a(k) · a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* static state for memoisation */
static seq_t *cache   = NULL;   /* a(k) values       */
static size_t cache_cap = 0;    /* allocated size    */
static idx_t  cache_used   = 0; /* highest n computed */

static seq_t *w_cache = NULL;   /* 1 + k^5            */
static size_t w_cap   = 0;      /* allocated size    */

static seq_t *conv    = NULL;   /* convolution array  */
static size_t conv_cap = 0;     /* allocated size    */

/* enlarge cache if needed */
static void ensure_cache(idx_t need)
{
    if (need < (idx_t)cache_cap) return;
    size_t new_cap = (size_t)need * 2 + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* enlarge weight table if needed – compute 1 + k⁵ correctly */
static void ensure_weights(idx_t up_to)
{
    if (up_to <= (idx_t)w_cap) return;
    size_t new_cap = (size_t)up_to + 1;
    seq_t *tmp = (seq_t *)realloc(w_cache, new_cap * sizeof(seq_t));
    w_cache = tmp;

    for (idx_t k = (idx_t)w_cap + 1; k <= up_to; ++k) {
        seq_t val = (seq_t)k;
        val *= k;   /* ×k  → k² */
        val *= k;   /* ×k  → k³ */
        val *= k;   /* ×k  → k⁴ */
        val *= k;   /* ×k  → k⁵ */
        w_cache[k] = val + 1;
    }
    w_cap = up_to;
}

/* enlarge convolution array if needed */
static void ensure_conv(idx_t max_n)
{
    /* we need indices up to 2*max_n (for worst case i+i)   */
    if (max_n * 2 + 1 <= conv_cap) return;
    size_t new_cap = (size_t)max_n * 2 + 1;
    seq_t *tmp = (seq_t *)realloc(conv, new_cap * sizeof(seq_t));
    conv = tmp;
    conv_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1             */
        cache_used = 0;

        w_cache = (seq_t *)calloc(1, sizeof(seq_t)); /* w[0] = 1 */
        w_cache[0] = (seq_t)1;
        w_cap = 0;

        conv = (seq_t *)calloc(1, sizeof(seq_t));    /* conv[0] = 1 */
        conv[0] = (seq_t)1;
        conv_cap = 1;
    }

    /* extend tables to compute up to n */
    ensure_cache(n);
    ensure_weights(n);
    ensure_conv(n);

    /* compute missing terms, if any */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        /* a(i) = 1 + conv[i-1] */
        seq_t ai = conv[(size_t)i - 1] + 1;
        cache[i] = ai;

        /* update convolution: add contributions involving a(i) */
        for (idx_t k = 0; k < i; ++k) {
            /* term with weight w_k */
            seq_t t1 = w_cache[k] * cache[k] * ai;
            conv[(size_t)k + i] += t1;
            /* term with weight w_i (symmetric counterpart) */
            seq_t t2 = w_cache[i] * ai * cache[k];
            conv[(size_t)k + i] += t2;
        }
        /* term where both indices are i: w_i * a(i)^2 */
        conv[(size_t)2 * i] += w_cache[i] * ai * ai;
    }

    cache_used = n;
    return cache[n];
}
