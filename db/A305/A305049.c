/*
 * A305049  Expansion of 1/(1 - Sum_{k>=1} tau_k(k)*x^k), where tau_k(k) =
 *          number of ordered k-factorizations of k (A163767).
 * https://oeis.org/A305049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=Σ_{k=1..n} τ_k(k)*a(n-k) where
 *            τ_k(k)=∏_{p^e||k} binom(e+k-1, k-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(n) is computed on demand and memoized. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

/* static data for memoization */
static seq_t *A = NULL;       /* a(n) values, indices 0..max_n */
static seq_t *F = NULL;       /* τ_k(k) values for k≥1, indices 0..max_n (F[0] unused) */
static idx_t  *spf = NULL;    /* smallest prime factor, indices 0..spf_cap */
static idx_t  spf_cap = 0;    /* maximum index in spf array */
static idx_t  max_n  = 0;     /* largest n that has been computed */

/* extend spf array up to 'limit' (inclusive) */
static void build_spf(idx_t limit)
{
    if (limit <= spf_cap) return;
    if (spf == NULL) {
        spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
        for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    } else {
        spf = (idx_t*)realloc(spf, (limit + 1) * sizeof(idx_t));
        for (idx_t i = spf_cap + 1; i <= limit; ++i) spf[i] = i;
    }
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (spf[i] == i) { /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_cap = limit;
}

/* compute τ_k(k) for a given k using the cached spf array */
static seq_t compute_tauk(idx_t k)
{
    /* compute multiplicatively over prime powers of k */
    seq_t prod = (seq_t)1;
    idx_t m = k;
    while (m > 1) {
        idx_t p = spf[m];
        idx_t e = 0;
        while ((m % p) == 0) {
            m /= p;
            ++e;
        }
        /* multiply by binomial(e + k - 1, k - 1) */
        prod = seq_mul(prod, binom(e + k - 1, k - 1));
    }
    return prod;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* first invocation: allocate structures up to n */
    if (A == NULL) {
        /* allocate arrays up to index n (inclusive) */
        A = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        F = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        spf = NULL;               /* will be built below */
        A[0] = (seq_t)1;          /* base case a(0)=1 */

        build_spf(n);             /* prepare spf for all k≤n */

        /* compute τ_k(k) for 1..n and then a(k) recursively */
        for (idx_t k = 1; k <= n; ++k) {
            F[k] = compute_tauk(k);
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= k; ++j)
                sum = sum + seq_mul(F[j], A[k - j]);
            A[k] = sum;
        }
        max_n = n;
        return A[n];
    }

    /* if already known, just return */
    if (n <= max_n) return A[n];

    /* otherwise extend computations to n */
    build_spf(n);

    /* grow arrays */
    A = (seq_t*)realloc(A, (n + 1) * sizeof(seq_t));
    F = (seq_t*)realloc(F, (n + 1) * sizeof(seq_t));

    /* compute new τ_k(k) and a(k) from max_n+1 up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        F[k] = compute_tauk(k);
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= k; ++j)
            sum = sum + seq_mul(F[j], A[k - j]);
        A[k] = sum;
    }

    max_n = n;
    return A[n];
}
