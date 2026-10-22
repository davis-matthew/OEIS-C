/*
 * A365897  Expansion of e.g.f. exp( Sum_{k>=0} x^(3*k+5) / (3*k+5)! ).
 * https://oeis.org/A365897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 22, u64 n <= 32, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365897 :  a(0)=1, a(n)=∑_{k=0..⌊(n-5)/3⌋} binomial(n-1,3k+4)*a(n-3k-5) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0u;

/* Helper to ensure the cache can store up to idx elements (idx==n+1)   */
static void ensure_cache(idx_t need, seq_t **p, size_t *cap)
{
    /* simple reallocate if needed – not thread‑safe, but OK for this challenge */
    if ((size_t)need > *cap) {
        size_t new_cap = need * 2;
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        *p = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        max_computed = 0u;
    }

    /* extend cache if n not yet computed */
    if (n > max_computed) {
        idx_t new_size = n + 1;
        ensure_cache(new_size, &cache, &cache_cap);

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 0;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i >= 5) {
                idx_t maxk = (i - 5) / 3;
                for (idx_t k = 0; k <= maxk; ++k) {
                    idx_t m = 3 * k + 4;           /* choose index in binomial */
                    seq_t bin = binomial(i - 1, m);
                    seq_t sub = cache[i - 3 * k - 5];
                    val = val + bin * sub;
                }
            }
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
