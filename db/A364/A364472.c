/*
 * A364472  G.f. satisfies A(x) = 1 + x*A(x) + x^2*A(x)^6.
 * https://oeis.org/A364472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binom(n+4*k,6*k) *
 *            binom(6*k,k)/(5*k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Cache for already computed a(n) values. */
static seq_t  *cache      = NULL;   /* array of a(n) values */
static idx_t   cache_len  = 0;     /* number of values currently cached */
static size_t  cache_cap  = 0;     /* allocated cache size */

/* For each k we store Ck = binom(6*k,k)/(5*k+1) */
static seq_t  *Ck         = NULL;   /* array of Ck */
static size_t  Ck_cap     = 0;     /* allocated Ck size */

/* binom_val[k] holds binom(current_n + 4*k, 6*k) for the current_n */
static seq_t  *binom_val  = NULL;   /* array of binomial values */
static size_t  binom_cap  = 0;     /* allocated binom_val size */

static idx_t   current_n  = 0;     /* last n that has been computed */

/* Ensure that cache has space for indices up to need_n (inclusive). */
static void ensure_cache_cap(idx_t need_n)
{
    if (need_n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)need_n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

/* Ensure that binom_val and Ck arrays have space for at least k_max indices. */
static void ensure_K_cap(idx_t k_max)
{
    if ((size_t)k_max + 1 > binom_cap) {
        size_t new_cap = (binom_cap == 0) ? 1 : binom_cap;
        while (new_cap <= (size_t)k_max) new_cap *= 2;
        binom_val = (seq_t *)realloc(binom_val, sizeof(seq_t) * new_cap);
        Ck        = (seq_t *)realloc(Ck,        sizeof(seq_t) * new_cap);
        for (size_t i = binom_cap; i < new_cap; ++i) {
            binom_val[i] = 0;
            Ck[i]        = 0;
        }
        binom_cap = new_cap;
    }
}

/* Compute Ck for the given k if it has not been computed yet. */
static void ensure_Ck(idx_t k)
{
    if (k == 0) {
        Ck[0] = 1;
        return;
    }
    if (Ck[(size_t)k] == 0) {
        seq_t num = binom(6 * k, k);          /* binom(6k, k) */
        seq_t den = (seq_t)(5 * k + 1);
        Ck[(size_t)k] = num / den;
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* If already cached, return it */
    if (n < cache_len) return cache[n];

    /* First call – initialise storage */
    if (cache_cap == 0) {
        ensure_cache_cap(n);
        cache[0] = 1;
        cache_len = 1;
        current_n = 0;

        idx_t k_max = n / 2;
        ensure_K_cap(k_max);
        Ck[0] = 1;          /* C_0 = 1 */
        binom_val[0] = 1;   /* binom(0,0) = 1 */
    }

    /* Compute values up to the requested n, increasing current_n step by step. */
    while (current_n < n) {
        idx_t next_n = current_n + 1;
        idx_t old_k_max = current_n / 2;
        idx_t new_k_max = next_n / 2;

        ensure_K_cap(new_k_max);

        /* Update existing binomial values for k <= old_k_max */
        for (idx_t k = 0; k <= old_k_max; ++k) {
            seq_t num = binom_val[(size_t)k] * (seq_t)(current_n + 4 * k + 1);
            seq_t den = (seq_t)(current_n + 1 - 2 * k);
            binom_val[(size_t)k] = num / den;
        }

        /* New k if the upper bound increased */
        if (new_k_max > old_k_max) {
            idx_t k = new_k_max;
            /* For k = next_n/2 we have binom(3*next_n, 3*next_n) = 1 */
            binom_val[(size_t)k] = 1;
            ensure_Ck(k);
        }

        /* Compute a(next_n) = sum_{k=0..new_k_max} binom_val[k] * Ck[k] */
        seq_t sum = 0;
        for (idx_t k = 0; k <= new_k_max; ++k) {
            if (Ck[(size_t)k] == 0) ensure_Ck(k);
            sum += binom_val[(size_t)k] * Ck[(size_t)k];
        }

        ensure_cache_cap(next_n);
        cache[next_n] = sum;

        current_n = next_n;
        cache_len = current_n + 1;
    }

    return cache[n];
}
