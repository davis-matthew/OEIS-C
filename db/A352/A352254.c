/*
 * A352254  Expansion of e.g.f. exp( x * sinh(x) / 2 ) (even powers only).
 * https://oeis.org/A352254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} binomial(2n-1, 2k-1) * k * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoisation to be efficient for any order of calls. */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) returns seq_t */

/* memoisation array */
static seq_t *cache = NULL;      /* allocated array of computed terms */
static size_t cache_cap = 0;     /* current capacity of cache */
static size_t cache_len = 0;     /* number of terms stored (equals cache_cap if full) */

/* enlarge the cache to hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }   /* unrecoverable failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* allow access to arbitrary indices in any order */
    ensure_capacity((size_t)n + 1);

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            /* binomial(2*i-1, 2*k-1) */
            idx_t num   = (idx_t)(2*i - 1);
            idx_t denom = (idx_t)(2*k - 1);
            seq_t binom = binomial(num, denom);
            seq_t term  = binom * (seq_t)k * cache[i - k];
            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
