/*
 * A337060  E.g.f.: 1 / (1 + x^2/2 + log(1 - x)).
 * https://oeis.org/A337060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; a(1) = 1; a(2) = 2;
 * a(n) = n * a(n-1)
 *        + Σ_{k=3..n} binomial(n, k) * (k-1)! * a(n-k)
 */

#include <stdlib.h>

/* utility functions provided by the build */
#include <stdint.h>
#include "../utils/mem.h"

/* Compute binomial coefficient n choose k multiplicatively.
   This works for any seq_t that supports + - * / % with integer constants. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)      /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static seq_t *cache = NULL;    /* cache of a(n) values */
static idx_t cache_len = 0;    /* number of cached terms (exclusive) */

static seq_t *fact_cache = NULL; /* cache of factorials (k)! */
static idx_t fact_len = 0;       /* number of cached factorials (exclusive) */

/* Ensure factorial cache contains (0)! … (n-1)! */
static void ensure_fact(idx_t n)
{
    if (fact_len > n) return;
    idx_t new_len = n + 1;                     /* need up to (n-1)! */
    seq_t *new_fact = mem_resize(seq_t, fact_cache, new_len);
    if (!new_fact) abort();
    fact_cache = new_fact;
    if (fact_len == 0) {
        fact_cache[0] = (seq_t)1;              /* 0! = 1 */
        fact_len = 1;
    }
    for (idx_t i = fact_len; i < new_len; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    fact_len = new_len;
}

/* Extend value cache up to n */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;
    idx_t new_len = n + 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
    if (!new_cache) abort();
    cache = new_cache;
    for (idx_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;

    /* Precompute factorials up to (n-1)! */
    ensure_fact(n);

    /* Compute terms from current cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)2;
        } else {
            seq_t sum = (seq_t)i * cache[i - 1];   /* n * a(n-1) */
            for (idx_t k = 3; k <= i; ++k) {
                seq_t bin = binomial(i, k);            /* C(i,k) */
                seq_t f   = fact_cache[k - 1];         /* (k-1)! */
                seq_t term = bin * f * cache[i - k];
                sum += term;
            }
            cache[i] = sum;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
