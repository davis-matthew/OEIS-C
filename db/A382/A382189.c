/*
 * A382189  Expansion of 1/(1 - 4 * Sum_{k>=0} x^(3^k))^(1/2).
 * https://oeis.org/A382189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A(x) = (1 - 4 * Σ_{k>=0} x^{3^k} )^{-1/2} The coefficients a(n)
 *            satisfy a(0) = 1 a(n) = ( d(n) - Σ_{k=1}^{n-1} a(k)a(n-k) ) / 2
 *            (n ≥ 1) where d(n) is the coefficient of 1/(1-4Σx^{3^k}) and
 *            d(0) = 1, d(n) = 4 * Σ_{p∈{3^k:3^k≤n}} d(n-p)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static state for memoisation */
static seq_t *a_cache   = NULL;   /* a[n] */
static seq_t *d_cache   = NULL;   /* d[n] = coeff of 1/(1-4Σx^{3^k}) */
static size_t cache_cap = 0;      /* allocated capacity (>0) */
static idx_t  max_computed = 0;   /* largest n whose a[n] and d[n] are ready */

/* enlarge caches and compute missing terms up to n */
static void ensure_cache(idx_t n)
{
    /* initialise on first use */
    if (cache_cap == 0) {
        cache_cap = 1;
        a_cache   = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        d_cache   = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache || !d_cache) return;  /* out of memory – caller must treat */
        a_cache[0] = (seq_t)1; /* a(0) = 1 */
        d_cache[0] = (seq_t)1; /* d(0) = 1 */
        max_computed = 0;
    }

    /* grow capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        d_cache = (seq_t*)realloc(d_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    if (n <= max_computed) return;          /* already computed */

    /* compute d[n] for n from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t p = 1; p <= i; p *= 3) {
            sum += d_cache[i - p];
        }
        d_cache[i] = sum * (seq_t)4;
    }

    /* compute a[n] for n from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t cross = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            cross += a_cache[k] * a_cache[i - k];
        }
        a_cache[i] = (d_cache[i] - cross) / (seq_t)2;
    }

    max_computed = n;
}

/* external function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */
    ensure_cache(n);
    return a_cache[n];
}
