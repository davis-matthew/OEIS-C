/*
 * A301388  G.f. A(x) satisfies: A(x) = x * (1 + x*A'(x)) / (1 - x*A'(x)).
 * https://oeis.org/A301388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑1)*a(n‑1) + Σ_{k=1}^{n‑1} k*a(k)*a(n‑k), a(1)=1
 *            (derived from A(x) = x*(1 + x*A'(x))/(1 - x*A'(x)) )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                       /* for size_t */
#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array */
#include "../utils/seq_free_array.h"      /* seq_free_array */

/* ------------------------------------------------------------------ */
/* Global cache: cache[n‑1] holds a(n) for n ≥ 1.                      */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;     /* array of computed values */
static idx_t  cache_max = 0;    /* highest n already computed (0 means none) */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure cache can hold at least 'need' terms (need ≥ 1). */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap : 1;
    while (newcap < (size_t)need) newcap <<= 1;
    seq_t *newcache = seq_alloc_new_array(newcap);
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_max; ++i) newcache[i] = cache[i];
        seq_free_array(cache);
    }
    cache     = newcache;
    cache_cap = newcap;
    /* cache_max unchanged – the caller will initialise new entries. */
}

/* Compute all terms up to n (inclusive).  Complexity O(n²). */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max) return;          /* already available */

    ensure_capacity(n);

    /* Initialise a(1) if this is the first call. */
    if (cache_max == 0) {
        cache[0] = 1;                    /* a(1) = 1 */
        cache_max = 1;
        if (n == 1) return;
    }

    /* Compute missing terms, starting after the last known one. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            /* term = k * a(k) * a(i‑k) */
            sum += (seq_t)k * cache[k - 1] * cache[i - k - 1];
        }
        cache[i - 1] = (seq_t)(i - 1) * cache[i - 2] + sum;
    }
    cache_max = n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but safe fallback */
    compute_up_to(n);
    return cache[n - 1];
}
