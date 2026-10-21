/*
 * A282253  Coefficients in q-expansion of E_6^3, where E_6 is the Eisenstein
 *          series shown in A013973.
 * https://oeis.org/A282253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i+j+k=n} coeff_i coeff_j coeff_k of (1 -504 Σ_{m≥1}
 *            σ_5(m)q^m)^3 where coeff_m = -504 σ_5(m) for m≥1 and coeff_0 =
 *            1. The code evaluates the coefficients for all n ≥ 0 on first
 *            use and memoises them for later calls
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 13, i128 n <= 181,
 *            big any n
 * Helpers:   big_pow, dealloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/dealloc_seq.h"

typedef uint64_t idx_t;   /* supplied by the build */

static seq_t *sigma5 = NULL;
static seq_t *conv2  = NULL;
static seq_t *conv3  = NULL;
static seq_t *coeff  = NULL;
static idx_t  max_n  = 0;

/* allocate an array of seq_t, compatible with C and C++ */
static seq_t *alloc_seq(idx_t n)
{
#if defined(__cplusplus)
    return new seq_t[n];
#else
    return (seq_t *)malloc(sizeof(seq_t) * n);
#endif
}

/* compute σ_5(1)…σ_5(N) by a divisor‑sieve */
static void compute_sigma5(idx_t N)
{
    for (idx_t i = 0; i <= N; ++i) sigma5[i] = 0;
    sigma5[0] = 0;
    for (idx_t d = 1; d <= N; ++d) {
        seq_t d5 = big_pow((seq_t)d, (seq_t)5);            /* d^5 */
        for (idx_t m = d; m <= N; m += d) sigma5[m] += d5; /* add divisor contribution */
    }
}

/* compute convolution sums  S2[n] = Σ σ5[i]σ5[n-i] (i,n-i≥1)
   and S3[n] = Σ σ5[i]σ5[j]σ5[n-i-j] (all indices≥1) */
static void compute_convolutions(idx_t N)
{
    for (idx_t i = 0; i <= N; ++i) conv2[i] = conv3[i] = 0;

    for (idx_t i = 1; i <= N; ++i)
        for (idx_t j = 1; i + j <= N; ++j)
            conv2[i + j] += sigma5[i] * sigma5[j];

    for (idx_t k = 3; k <= N; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= k - 2; ++i)
            sum += sigma5[i] * conv2[k - i];
        conv3[k] = sum;
    }
}

/* compute coefficients of E_6^3 up to N */
static void compute_coefficients(idx_t N)
{
    coeff[0] = 1;
    seq_t sq504   = (seq_t)504 * (seq_t)504;               /* 504^2   */
    seq_t c3      = -((seq_t)504 * (seq_t)504 * (seq_t)504);/* -504^3  */

    for (idx_t n = 1; n <= N; ++n) {
        seq_t a = -(seq_t)1512 * sigma5[n];                  /* 3*(-504)σ5(n) */
        a += (seq_t)3 * sq504 * conv2[n];                   /* 3*504^2*Σσ5σ5 */
        a += c3 * conv3[n];                                 /* (-504)^3*Σσ5σ5σ5 */
        coeff[n] = a;
    }
}

/* main accessor */
seq_t a(idx_t n)
{
    if (n >= max_n) {
        idx_t new_size = n + 1;

        /* allocate new larger arrays */
        seq_t *new_sigma = alloc_seq(new_size);
        seq_t *new_conv2  = alloc_seq(new_size);
        seq_t *new_conv3  = alloc_seq(new_size);
        seq_t *new_coeff  = alloc_seq(new_size);

        /* copy old data if any */
        if (max_n > 0) {
            for (idx_t i = 0; i < max_n; ++i) {
                new_sigma[i] = sigma5[i];
                new_conv2[i] = conv2[i];
                new_conv3[i] = conv3[i];
                new_coeff[i] = coeff[i];
            }
        }
        /* free old arrays */
        if (sigma5) dealloc_seq(sigma5);
        if (conv2)  dealloc_seq(conv2);
        if (conv3)  dealloc_seq(conv3);
        if (coeff)  dealloc_seq(coeff);

        sigma5 = new_sigma;
        conv2  = new_conv2;
        conv3  = new_conv3;
        coeff  = new_coeff;

        compute_sigma5(n);
        compute_convolutions(n);
        compute_coefficients(n);

        max_n = new_size;
    }
    return coeff[n];
}
