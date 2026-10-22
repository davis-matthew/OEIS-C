/*
 * A345749  E.g.f.: Product_{k>=1} 1/(1 - (exp(x) - 1)^k)^(1/k).
 * https://oeis.org/A345749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * A028342(k) Equivalent
 *            recurrence: a(0) = 1 for n >= 1: a(n) = Σ_{k=1..n}
 *            binomial(n-1,k-1) * g_k * a(n-k) where g_k = Σ_{d=1..k} (d-1)! *
 *            d(d) * S(k,d) S(m,k) are Stirling numbers of the second kind
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

static idx_t cap      = 0;      /* maximum index currently computed          */
static seq_t *g_arr   = NULL;   /* g_k   for k <= cap                       */
static seq_t *a_arr   = NULL;   /* a_k   for k <= cap                       */
static seq_t *fac_arr = NULL;   /* factorials 0..cap                        */
static idx_t *divcnt  = NULL;   /* divisor counts 0..cap                    */
static seq_t *S_arr   = NULL;   /* current Stirling row (size cap+1)         */

/* Extend all tables to at least index `n` and compute missing values. */
static void ensure_cap(idx_t n)
{
    if (cap >= n) return;

    idx_t old_cap = cap;
    idx_t new_cap = n;

    /* resize arrays */
    g_arr   = (seq_t*)realloc(g_arr,   (new_cap + 1) * sizeof(seq_t));
    a_arr   = (seq_t*)realloc(a_arr,   (new_cap + 1) * sizeof(seq_t));
    fac_arr = (seq_t*)realloc(fac_arr, (new_cap + 1) * sizeof(seq_t));
    divcnt  = (idx_t*)realloc(divcnt,  (new_cap + 1) * sizeof(idx_t));
    S_arr   = (seq_t*)realloc(S_arr,   (new_cap + 1) * sizeof(seq_t));

    /* initialize newly allocated part */
    if (old_cap == 0) {
        a_arr[0] = (seq_t)1;          /* a(0) */
        g_arr[0] = (seq_t)0;          /* unused, but set */
        fac_arr[0] = (seq_t)1;        /* 0! */
        S_arr[0] = (seq_t)1;          /* S(0,0) */
        divcnt[0] = 0;
    }
    /* zero S beyond old_cap */
    for (idx_t i = old_cap + 1; i <= new_cap; ++i) {
        S_arr[i] = (seq_t)0;
    }

    /* divisor counts up to new_cap */
    for (idx_t i = 1; i <= new_cap; ++i) divcnt[i] = 0;
    for (idx_t i = 1; i <= new_cap; ++i) {
        for (idx_t j = i; j <= new_cap; j += i) divcnt[j] += 1;
    }

    /* factorials up to new_cap */
    for (idx_t i = old_cap + 1; i <= new_cap; ++i)
        fac_arr[i] = fac_arr[i-1] * (seq_t)i;

    /* compute g_k and a_k for k = old_cap+1 .. new_cap */
    for (idx_t m = old_cap + 1; m <= new_cap; ++m) {
        /* update S to row m */
        S_arr[m] = (seq_t)0;                 /* S(m,m) start from 0 */
        for (int k = (int)m; k >= 1; --k) {
            seq_t old_val = S_arr[k];
            seq_t left_val = S_arr[k-1];     /* S(m-1,k-1) */
            S_arr[k] = left_val + (seq_t)k * old_val;
        }
        S_arr[0] = (seq_t)0;                 /* S(m,0) = 0 */

        /* compute g_m */
        seq_t gm = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = fac_arr[k-1];
            term *= (seq_t)divcnt[k];
            term *= S_arr[k];
            gm += term;
        }
        g_arr[m] = gm;

        /* compute a_m using recurrence a(m) = Σ_{k=1..m} C(m-1,k-1)*g_k*a(m-k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t coeff = binomial(m-1, k-1);
            coeff *= g_arr[k];
            coeff *= a_arr[m-k];
            sum += coeff;
        }
        a_arr[m] = sum;
    }

    cap = new_cap;
}

/* Public function as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cap(n);
    return a_arr[n];
}
