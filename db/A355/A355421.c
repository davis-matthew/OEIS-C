/*
 * A355421  Expansion of e.g.f. exp(Sum_{k=1..3} (exp(k*x) - 1)).
 * https://oeis.org/A355421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} (1 + 2^k + 3^k) * C(n-1, k-1) *
 *            a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A355421.  Uses memoisation with a static cache. */

#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build environment. */
extern seq_t a(idx_t n);
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache has room for indices up to `n` (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                      /* OOM: silently fail (value remains 0) */

    /* Zero any new entries. */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    cache = tmp;
    cache_cap = new_cap;
}

/* The main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_cache(n);
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;  /* initialise a(0) */

    /* Compute values up to `n` if necessary. */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != (seq_t)0) continue;

        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;       /* C(i-1, k-1) for current k */
        seq_t pow2  = (seq_t)2;       /* 2^k  (k will start at 1) */
        seq_t pow3  = (seq_t)3;       /* 3^k  (k will start at 1) */

        for (size_t k = 1; k <= i; ++k) {
            seq_t term = (seq_t)1 + pow2 + pow3;  /* (1 + 2^k + 3^k) */
            term = term * binom;
            term = term * cache[i - k];
            sum += term;

            if (k < i) {
                /* Update binom for the next k:
                   binom(i-1, k) = binom(i-1, k-1) * (i-k)/k
                */
                binom = binom * (seq_t)(i - k) / (seq_t)k;
                pow2 *= (seq_t)2;
                pow3 *= (seq_t)3;
            }
        }
        cache[i] = sum;
    }

    return cache[(size_t)n];
}
