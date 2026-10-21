/*
 * A293037  E.g.f.: exp(1 + x - exp(x)).
 * https://oeis.org/A293037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=0}^{n-2} binomial(n-1,k) * a(k) with a(0) = 1, a(1)
 *            = 0 Recurrence implied by the e.g.f. exp(1 + x - exp(x))
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 28, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
/* C++ version: use std::vector to store the cache. */
#include <vector>
static std::vector<seq_t> cache;
#endif

/* ------------------------------------------------------------------------ */
/* Helper: compute binomial coefficient C(n,k) for idx_t n,k using seq_t.   */
/* ------------------------------------------------------------------------ */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* ------------------------------------------------------------------------ */
/* Dynamic cache (C version).                                               */
/* ------------------------------------------------------------------------ */
#ifndef __cplusplus
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of elements really stored   */
static size_t cache_cap  = 0;   /* allocated capacity                 */

static void ensure_capacity_c(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while ((size_t)n + 1 > new_cap) new_cap *= 2;
    seq_t *new_mem = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) abort();      /* out of memory */
    cache = new_mem;
    cache_cap = new_cap;
}
#endif

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* -------------------------------------------------------------------- */
    /* C++ implementation with std::vector cache.                           */
    /* -------------------------------------------------------------------- */
    if (n >= cache.size()) {
        cache.resize(n + 1);
        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 0;
            } else {
                seq_t sum = 0;
                seq_t comb = 1;                /* binomial(i-1,0) */
                for (idx_t k = 0; k <= i - 2; ++k) {
                    sum += comb * cache[k];
                    /* update comb to binomial(i-1, k+1) */
                    comb = comb * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
                }
                cache[i] = -sum;
            }
        }
    }
    return cache[n];
#else
    /* -------------------------------------------------------------------- */
    /* C implementation with malloc/realloc cache.                           */
    /* -------------------------------------------------------------------- */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    ensure_capacity_c(n);
    if (cache_size <= n) {
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)0;
            } else {
                seq_t sum = 0;
                seq_t comb = 1;                 /* binomial(i-1,0) */
                for (idx_t k = 0; k <= i - 2; ++k) {
                    sum += comb * cache[k];
                    comb = comb * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
                }
                cache[i] = -sum;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
#endif
}
