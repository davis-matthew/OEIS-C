/*
 * A318355  E.g.f. satisfies y'' + y' - x^3*y = 0 with y(0)=1, y'(0)=0.
 * https://oeis.org/A318355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 19, i64 n <= 30, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n+5) = (n+3)*(n+2)*(n+1)*a(n) - a(n+4)   for n ≥ 0
 * Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Memoisation cache for computed terms */
static seq_t *cache = NULL;   /* array of terms   */
static idx_t  cache_size = 0; /* number of filled indices (max index + 1) */
static idx_t  cache_cap   = 0; /* allocated capacity */

/* Ensure the cache can hold indices up to 'n' (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t newcap = (cache_cap ? cache_cap << 1 : 8);
    while (newcap <= n)
        newcap <<= 1;
    seq_t *newc = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newc)
        abort();
    if (cache) {
        memcpy(newc, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = newc;
    cache_cap = newcap;
}

/* Return the n‑th term of sequence A318355. */
seq_t a(idx_t n)
{
    /* initialise with known values on first call */
    if (!cache) {
        ensure_capacity(4);          /* indices 0..4 */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = 0;
        cache[0] = 1;
        cache_size = 5;              /* values 0..4 stored */
    }

    if (n < cache_size)
        return cache[n];

    /* need to compute up to index n */
    ensure_capacity(n);

    /* start from the smallest k for which a(k+5) is not yet known */
    idx_t start = (cache_size > 5) ? (cache_size - 5) : 0;

    /* compute terms a(k+5) for k = start .. such that k+5 ≤ n */
    for (idx_t k = start; k + 5 <= n; ++k) {
        seq_t coeff = (seq_t)(k + 3) * (k + 2) * (k + 1); /* (k+3)(k+2)(k+1) */
        seq_t prod  = cache[k] * coeff;                   /* a(k) * coeff */
        seq_t val   = prod - cache[k + 4];                 /* a(k+5) */
        cache[k + 5] = val;
    }

    cache_size = n + 1;  /* indices 0..n are now stored */
    return cache[n];
}
