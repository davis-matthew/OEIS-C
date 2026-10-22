/*
 * A346983  Expansion of e.g.f. 1 / (5 - 4 * exp(x))^(1/4).
 * https://oeis.org/A346983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346983
   Recurrence: a(0) = 1
   a(n) = Σ_{k=1..n} (4*n-3*k)/n * binomial(n,k) * a(n-k)
   on 0‑based indexing (a(0) is the first term). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t cache_sz = 0;

/* Allocate or resize the cache to length n+1.  Newly added
   entries are computed using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;      /* a(0) */
        cache_sz = 1;
    }
    if (cache_sz > n) return;        /* already computed */

    idx_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) { free(cache); exit(1); }
    cache = tmp;
    /* Compute missing terms */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t binom = binomial((idx_t)i, k);      /* C(i,k) */
            seq_t num   = (seq_t)(4 * i - 3 * k);     /* 4*i-3*k   */
            seq_t term  = (num * binom) / (seq_t)i;   /* (4i-3k)/i * C(i,k) */
            sum += term * cache[i - k];
        }
        cache[i] = sum;
    }
    cache_sz = new_sz;
}

/* Public API – returns the n‑th term of A346983. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
