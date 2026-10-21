/*
 * A293049  Expansion of e.g.f. exp(x^3/(1 - x)).
 * https://oeis.org/A293049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=6, for n>3: a(n)=2*(n‑1)*a(n-1)-(n‑1)*(n‑2)*a(n-2)
 *            +6*C(n‑1,2)*a(n-3)-12*C(n‑1,3)*a(n-4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* initialise cache once */
    if (cache == NULL) {
        /* initial capacity 4: a(0)..a(3) */
        cache = seq_realloc(NULL, 0, 4);
        cache_cap = 4;
        cache_len = 4;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)6;
    }
    /* grow capacity if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap)
            new_cap *= 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 3) continue;                 /* already initialised */
        seq_t term1 = (seq_t)2 * (seq_t)(i-1) * cache[i-1];
        seq_t term2 = (seq_t)(i-1) * (seq_t)(i-2) * cache[i-2];
        seq_t term3 = (seq_t)6 * binomial(i-1, 2) * cache[i-3];
        seq_t term4 = (seq_t)12 * binomial(i-1, 3) * cache[i-4];
        cache[i] = term1 - term2 + term3 - term4;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
