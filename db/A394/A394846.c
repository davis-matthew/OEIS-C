/*
 * A394846  G.f. A(x) satisfies A(x) = 1 / (1 - x/5 * d/dx log(1 + 5*x*A(x))).
 * https://oeis.org/A394846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = 5*a(n-1) + (n-9)/2 * sum_{k=0..n-1}
 *            a(k)*a(n-1-k) (n ≥ 1)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Cached computation of the sequence                                 */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;      /* array of computed values      */
static size_t cache_cap = 0;         /* allocated capacity           */
static idx_t last_computed = 0;      /* highest index already filled */

/* Ensure that seq_cache[0..n] are computed and stored. */
static void ensure_cache(idx_t n)
{
    /* Initial allocation and base value */
    if (seq_cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 8) cache_cap = 8;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) return;  /* allocation failure: caller will crash on the next access */
        seq_cache[0] = (seq_t)1;   /* a(0) = 1 */
        last_computed = 0;
    }

    /* Grow capacity if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;  /* allocation failure: caller will crash on the next access */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* convolution sum: sum_{k=0..i-1} a(k)*a(i-1-k) */
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j) {
            sum += seq_cache[j] * seq_cache[i - 1 - j];
        }
        seq_t coeff   = (seq_t)i - (seq_t)9;
        seq_t contrib = coeff * sum / 2;          /* integer division */
        seq_cache[i] = 5 * seq_cache[i - 1] + contrib;
    }
    last_computed = n;
}

/* ------------------------------------------------------------------ */
/* Public interface required by the framework                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
