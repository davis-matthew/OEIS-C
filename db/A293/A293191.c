/*
 * A293191  a(n) = n! * [x^n] exp(n*arcsin(x)).
 * https://oeis.org/A293191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A293191:  a(n) = 2 * Σ_{k=1}^{n-1} binomial(n,k) * a(k) * a(n-k-1),
    with a(0)=a(1)=1.  The values are memoised in a static cache so
    calls in any order are efficient.  All arithmetic is performed in
    the user‑supplied integer type seq_t.  The function works for any
    build (arbitrary precision or native integer). */

#include <stdlib.h>
#include <string.h>   /* for memset */

/* Static cache of already computed terms. */
static seq_t *a_cache   = NULL;
static size_t cache_cap = 0;
static idx_t  cached_upto = 1;   /* indices 0 .. 1 have been initialised */

/* Ensure the cache can hold indices up to 'n' and initialise missing
   entries to zero.  The function is called only by a() before
   accessing a_cache[n]. */
static void ensure_cache(idx_t n) {
    if (cache_cap <= (size_t)n) {
        /* Allocate at least n+1 elements, double size for amortisation. */
        size_t new_cap = (n + 1 > cache_cap * 2) ? (n + 1) : (cache_cap * 2);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* Out of memory            */
        if (!a_cache) {
            memset(tmp, 0, new_cap * sizeof(seq_t));
        } else {
            /* New memory must be zeroed. */
            memset(tmp + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }
        a_cache   = tmp;
        cache_cap = new_cap;
    }
    /* Initialise base cases if not already set. */
    if (a_cache[0] == 0) a_cache[0] = (seq_t)1;
    if (cache_cap > 1 && a_cache[1] == 0) a_cache[1] = (seq_t)1;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Ensure cache memory for n.  This will also initialise 0 and 1. */
    ensure_cache(n);

    /* If the requested value is already known, return it. */
    if (cached_upto >= n) return a_cache[n];

    /* Compute any missing terms up to 'n' in increasing order. */
    for (idx_t m = cached_upto + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)m;        /* C(m,1) */
        for (idx_t k = 1; k < m; ++k) {
            /* term = binomial(m,k) * a(k) * a(m-k-1) */
            seq_t prod = bin * a_cache[k] * a_cache[m - k - 1];
            sum += prod;
            /* Compute binomial(m,k+1) for next iteration, unless k==m-1. */
            if (k != m - 1) {
                bin = (bin * (seq_t)(m - k)) / (k + 1);
            }
        }
        a_cache[m] = (seq_t)2 * sum;
    }
    cached_upto = n;
    return a_cache[n];
}
