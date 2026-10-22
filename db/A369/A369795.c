/*
 * A369795  Binomial transform of A355408.
 * https://oeis.org/A369795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} (3^k - 1) * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow3, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;         /* stored values a(0)…a(max) */
static size_t cache_len = 0;        /* number of values already computed */
static size_t cache_cap = 0;        /* allocated capacity of cache */

static seq_t *pow3_cache = NULL;    /* 3^i for i = 0…max */
static size_t pow3_len = 0;         /* number of computed powers */
static size_t pow3_cap = 0;         /* allocated capacity of pow3_cache */

/* Ensure that a(0)…a(n) and 3^0…3^n are available. */
static void ensure_up_to(idx_t n)
{
    size_t need = (size_t)n + 1;
    /* expand cache if necessary */
    if (need > cache_cap) {
        if (cache == NULL) {
            cache_cap = need;
            cache     = seq_alloc(cache_cap);
        } else {
            cache_cap = need;
            cache     = seq_realloc(cache, cache_cap / need * cache_cap, cache_cap);
        }
    }
    /* expand pow3 array if necessary */
    if (need > pow3_cap) {
        if (pow3_cache == NULL) {
            pow3_cap   = need;
            pow3_cache = seq_alloc(pow3_cap);
        } else {
            pow3_cap   = need;
            pow3_cache = seq_realloc(pow3_cache, pow3_cap / need * pow3_cap, pow3_cap);
        }
    }
    /* compute new 3^i values */
    for (size_t i = pow3_len; i < need; ++i) {
        if (i == 0)
            pow3_cache[0] = (seq_t)1;
        else
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
    }
    pow3_len = need;

    /* initialise the first term if this is the very first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute new a(i) values */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)1;
        for (idx_t j = 0; j < i; ++j) {
            seq_t term   = binomial((idx_t)i, (idx_t)j);
            seq_t factor = pow3_cache[(size_t)(i - j)] - (seq_t)1;
            term   *= factor;
            term   *= cache[(size_t)j];
            sum    += term;
        }
        cache[(size_t)i] = sum;
    }
    cache_len = need;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
