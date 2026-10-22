/*
 * A321989  Expansion of e.g.f. exp(exp(-x)/(1 - x) - 1).
 * https://oeis.org/A321989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 ; a(n) = Sum_{k=1..n} der(k) * binomial(n-1, k-1) *
 *            a(n-k) where der(k) is the number of derangements (subfactorial)
 *            of k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external includes beyond those needed for memory and vector handling. */
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#endif

/*--------------------------------------------------------------------*/
/*  Internal cached storage                                           */
/*--------------------------------------------------------------------*/
#ifndef __cplusplus
/* C implementation – use malloc / realloc */
static seq_t *der_cache = NULL;    /* der values 0..n */
static size_t der_cap   = 0;
static size_t der_size  = 0;        /* number of der values currently stored */

static seq_t *a_cache   = NULL;    /* a values 0..n */
static size_t a_cap     = 0;
static size_t a_size    = 0;        /* number of a values currently stored */
#else
/* C++ implementation – use std::vector */
static std::vector<seq_t> der_cache;
static size_t der_size = 0;        /* number of der values currently stored */

static std::vector<seq_t> a_cache;
static size_t a_size = 0;          /* number of a values currently stored */
#endif

/*--------------------------------------------------------------------*/
/*  Ensure der_cache holds values up to index n                        */
/*--------------------------------------------------------------------*/
static void ensure_der_cache_idx(idx_t n) {
    if (n < der_size) return;
#ifdef __cplusplus
    der_cache.resize(n + 1);
    for (idx_t i = der_size; i <= n; ++i) {
        if (i == 0) der_cache[i] = (seq_t)1;
        else if (i == 1) der_cache[i] = (seq_t)0;
        else der_cache[i] = (seq_t)(i - 1) * (der_cache[i - 1] + der_cache[i - 2]);
    }
    der_size = n + 1;
#else
    size_t new_cap = der_cap ? der_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(der_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    der_cache = tmp;
    der_cap = new_cap;
    memset(der_cache + der_size, 0,
           (der_cap - der_size) * sizeof(seq_t));
    for (idx_t i = der_size; i <= n; ++i) {
        if (i == 0) der_cache[i] = (seq_t)1;
        else if (i == 1) der_cache[i] = (seq_t)0;
        else der_cache[i] = (seq_t)(i - 1) * (der_cache[i - 1] + der_cache[i - 2]);
    }
    der_size = n + 1;
#endif
}

/*--------------------------------------------------------------------*/
/*  Ensure a_cache holds values up to index n                         */
/*--------------------------------------------------------------------*/
static void ensure_a_cache_idx(idx_t n) {
    if (n < a_size) return;
#ifdef __cplusplus
    a_cache.resize(n + 1);
    for (idx_t i = a_size; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* binomial(n-1,0) */
            for (idx_t k = 1; k <= i; ++k) {
                if (k > 1) {
                    /* binom = binom * (i - k + 1) / (k - 1) */
                    binom = binom * (seq_t)(i - k + 1) / (seq_t)(k - 1);
                }
                seq_t term = der_cache[k] * binom;
                term = term * a_cache[i - k];
                sum += term;
            }
            a_cache[i] = sum;
        }
    }
    a_size = n + 1;
#else
    size_t new_cap = a_cap ? a_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cap = new_cap;
    memset(a_cache + a_size, 0,
           (a_cap - a_size) * sizeof(seq_t));
    for (idx_t i = a_size; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;
            for (idx_t k = 1; k <= i; ++k) {
                if (k > 1) {
                    binom = binom * (seq_t)(i - k + 1) / (seq_t)(k - 1);
                }
                seq_t term = der_cache[k] * binom;
                term = term * a_cache[i - k];
                sum += term;
            }
            a_cache[i] = sum;
        }
    }
    a_size = n + 1;
#endif
}

/*--------------------------------------------------------------------*/
/*  Public function                                                  */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n) {
    /* Make sure we have all necessary ingredients up to n */
    ensure_der_cache_idx(n);
    ensure_a_cache_idx(n);
    /* Return the cached value */
#ifdef __cplusplus
    return a_cache[n];
#else
    return a_cache[n];
#endif
}
