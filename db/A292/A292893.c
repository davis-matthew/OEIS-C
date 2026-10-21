/*
 * A292893  Expansion of e.g.f. exp(x * (1 - exp(x))).
 * https://oeis.org/A292893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 24, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A292893: Expansion of e.g.f. exp(x * (1 - exp(x)))
 * Recurrence used:
 *   a(0) = 1, a(1) = 0
 *   a(n) = - Σ_{k=2..n} k * binomial(n-1, k-1) * a(n-k)
 *   (for all n ≥ 2)
 *
 * The sequence is computed lazily and memoised in a static cache.
 * Each term is formed using only integer arithmetic of type seq_t.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* dynamic array helpers for the cache                              */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* cache array for a(0..N)          */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* capacity of the cache array      */

/* Resize the cache to hold at least `needed` elements.               */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The cache must accommodate index n (0‑based). */
    if ((size_t)n + 1 > cache_len) {
        /* Extend the cache for all indices from cache_len up to n. */
        size_t new_len = (size_t)n + 1;
        ensure_cache_cap(new_len);

        /* Compute missing terms one by one. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)0;
            } else {
                /* a(i) = - Σ_{k=2..i}  k * C(i-1, k-1) * a(i-k) */
                seq_t sum = (seq_t)0;

                /* bin will hold C(i-1, k-1) for the current k. */
                seq_t bin = (seq_t)(i - 1);   /* C(i-1, 1) */

                for (idx_t k = 2; k <= i; ++k) {
                    /* term = k * a(i-k) * bin */
                    seq_t term = cache[i - k] * (seq_t)k;
                    term *= bin;
                    sum += term;

                    /* Update bin to C(i-1, k) for next iteration,
                       but only if we are not at the last k. */
                    if (k < i) {
                        seq_t num = (seq_t)(i - k);   /* (i-k)   */
                        seq_t den = (seq_t)k;        /* k       */
                        bin = bin * num / den;       /* exact division */
                    }
                }

                cache[i] = -sum;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
