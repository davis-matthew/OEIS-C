/*
 * A281231  Exponential transform of the tetrahedral numbers (A000292).
 * https://oeis.org/A281231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} binom(n-1,k-1) * T_k * a(n-k) , * with T_k =
 *            k*(k+1)*(k+2)/6 and a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponential transform of the tetrahedral numbers (A281231) */

#include <stdlib.h>
#include <stdint.h>

/* compute the k‑th tetrahedral number T_k = binom(k+2,3) */
static inline seq_t tetrahedral(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * (kk + (seq_t)1) * (kk + (seq_t)2) / (seq_t)6;
}

/* allocate or enlarge the cache array to hold at least n+1 elements */
static void ensure_cache(idx_t n, seq_t **p, size_t *cap)
{
    size_t need = (size_t)n + 1;
    if (!*p || *cap < need) {
        void *tmp = realloc(*p, need * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        *p   = (seq_t *)tmp;
        *cap = need;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of a(0)..a(max_computed) */
    static size_t cap   = 0;         /* current allocated capacity   */
    static idx_t max_computed = (idx_t)-1;  /* last matched index, ULLONG_MAX if none */

    /* Ensure cache exists and a(0) is set */
    if (cache == NULL) {
        ensure_cache(0, &cache, &cap);
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* If the requested n is already stored, return it */
    if (n <= max_computed) return cache[n];

    /* Expand cache if needed */
    ensure_cache(n, &cache, &cap);

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;   /* binom(i-1,0) */

        /* k goes from 1 to i */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = bin * tetrahedral(k) * cache[i - k];
            sum += term;
            if (k < i) {
                /* Update bin to binom(i-1,k) from binom(i-1,k-1) */
                seq_t mult = (seq_t)(i - k);   /* (i-1)-(k-1) = i-k */
                seq_t div  = (seq_t)k;
                bin = bin * mult / div;        /* exact division, no remainder */
            }
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
