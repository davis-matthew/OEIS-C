/*
 * A309775  Expansion of e.g.f. exp(2 * (1 - exp(x)) + x).
 * https://oeis.org/A309775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = a(n-1) - 2 * Sum_{k=0..n-1} binomial(n-1,k) *
 *            a(k) for n > 0 (computed with binomial(n-1,k) = (n-1 choose k))
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms   */
static size_t cache_cap = 0;     /* allocated size of cache   */
static size_t cache_len = 0;     /* number of terms already computed */

/* Resize the cache so that it can hold indices up to n and compute
   any missing terms.  Uses the recurrence above with an on‑the‑fly
   computation of binomial coefficients to avoid repeated calls. */
static void ensure_cache(idx_t n)
{
    /* already computed? */
    if (cache_len > (size_t)n)
        return;

    /* grow the cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t newcap = cache_cap ? cache_cap * 2 : 1;
        while (newcap <= (size_t)n)
            newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* compute missing terms */
    for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
        idx_t i = (idx_t)idx;
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        seq_t binom = 1;  /* C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            sum += binom * cache[(size_t)k];

            /* update binomial for next k: C(i-1,k+1) = C(i-1,k)*(i-1-k)/(k+1) */
            if (k + 1 < i) {
                seq_t num = (seq_t)(i - 1 - k);
                seq_t den = (seq_t)(k + 1);
                binom = binom * num / den;
            }
        }

        cache[i] = cache[i - 1] - (seq_t)2 * sum;
    }

    cache_len = (size_t)n + 1;
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
