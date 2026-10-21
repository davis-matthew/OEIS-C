/*
 * A277506  Expansion of e.g.f. 1/((1+LambertW(-x))*(1-x)).
 * https://oeis.org/A277506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n^n, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation with memoisation. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Cache for computed values, zero‑based. */
static seq_t *cache      = NULL;   /* array of seq_t */
static size_t cache_size = 0;      /* number of entries that are filled */
static size_t cache_cap  = 0;      /* current allocation capacity */

/* Ensure capacity for indices 0..n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    /* Base term a(0) = 1. */
    if (n == (idx_t)0) {
        if (cache_cap < 1) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t));
            cache = tmp;
            cache_cap = 1;
        }
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        return cache[0];
    }

    /* Ensure we have space for all required indices. */
    ensure_cache(n);

    /* Compute all missing values up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t pow_val = seq_pow((seq_t)i, (idx_t)i);            /* i^i */
        seq_t term    = ((seq_t)i) * cache[(size_t)(i - 1)] + pow_val; /* i*a(i-1)+i^i */
        cache[(size_t)i] = term;
    }

    cache_size = (size_t)(n + 1);
    return cache[(size_t)n];
}
