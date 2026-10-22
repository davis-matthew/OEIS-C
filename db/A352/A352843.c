/*
 * A352843  Expansion of e.g.f. exp(Sum_{k>=1} sigma_k(k) * x^k/k!).
 * https://oeis.org/A352843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} sigma_k(k) * binomial(n-1,k-1) * a(n-k) ,
 *            a(0) = 1. Where sigma_k(m) = sum_{d|m} d^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */

static seq_t compute_sigma_eq(idx_t k)
{
    if (k <= 1u) return (seq_t)1;   /* sigma_1(1) = 1 */

    seq_t result = 1;
    idx_t n = k;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            seq_t pk = seq_pow((seq_t)p, k);   /* p^k  */
            seq_t sum = 1;
            seq_t pow = 1;
            for (idx_t j = 1; j <= e; ++j) {
                pow = pow * pk;   /* p^(j*k) */
                sum += pow;
            }
            result *= sum;
        }
    }
    if (n > 1) {  /* remaining prime factor */
        seq_t pk = seq_pow((seq_t)n, k);
        seq_t sum = 1 + pk;
        result *= sum;
    }
    return result;
}

/* --- cache for a(n) -- */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;
static size_t a_cache_size = 0;

/* --- cache for sigma_k(k) (sigma_k(k, k) ) -- */
static seq_t *sigma_cache      = NULL;
static size_t sigma_cache_cap  = 0;
static size_t sigma_cache_size = 0;

/* Ensure cache arrays can hold index 'idx' (zero‑based). */
static void ensure_a_cap(idx_t idx)
{
    if (a_cache_cap <= idx) {
        size_t new_cap = (size_t)(idx + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* copy existing data */
        for (size_t i = 0; i < a_cache_cap; ++i) new_cache[i] = a_cache[i];
        /* zero new elements */
        for (size_t i = a_cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        free(a_cache);
        a_cache      = new_cache;
        a_cache_cap  = new_cap;
    }
}

static void ensure_sigma_cap(idx_t idx)
{
    if (sigma_cache_cap <= idx) {
        size_t new_cap = (size_t)(idx + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < sigma_cache_cap; ++i) new_cache[i] = sigma_cache[i];
        for (size_t i = sigma_cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        free(sigma_cache);
        sigma_cache      = new_cache;
        sigma_cache_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_a_cap(n);
    ensure_sigma_cap(n);

    /* initialise base values if this is the first call */
    if (a_cache_size == 0) {
        a_cache[0] = 1;
        if (a_cache_cap > 1) a_cache[1] = 1;
        a_cache_size = (a_cache_cap > 1) ? 2 : 1;

        if (sigma_cache_cap > 1) sigma_cache[1] = 1;
        sigma_cache_size = (sigma_cache_cap > 1) ? 2 : 1;
    }

    /* compute a(i) for i = a_cache_size .. n */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1; /* C(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* get sigma_k(k) value, compute if necessary */
            if (sigma_cache_size <= k) {
                sigma_cache[k] = compute_sigma_eq(k);
                sigma_cache_size = (size_t)k + 1;
            }

            seq_t term = sigma_cache[k] * binom * a_cache[i - k];
            sum += term;

            if (k < i) {
                /* binom = C(i-1, k) from C(i-1, k-1) */
                binom = binom * (seq_t)(i - k) / (seq_t)k;
            }
        }
        a_cache[i] = sum;
    }

    a_cache_size = (size_t)n + 1;
    return a_cache[n];
}
