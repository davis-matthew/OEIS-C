/*
 * A334243  a(n) = exp(n) * Sum_{k>=0} (k + n)^n * (-n)^k / k!.
 * https://oeis.org/A334243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] exp(n*(1 + x - exp(x)))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by summing a(n) = Sum_{k=0..n} binom(n,k) * Bell_k(-n) * n^(n-k) */
/*  Bell_k(x) is the complete Bell polynomial: Bell_{k+1}(x) = x * sum_{j=0}^k binom(k,j) * Bell_j(x) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special case n == 0 handled automatically by the loops */
    size_t sz = (size_t)n + 1U;                /* array size */

    /* allocate arrays for Bell polynomials and powers of n */
    seq_t *B     = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *pow_n = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!B || !pow_n) {
        free(B); free(pow_n);
        return (seq_t)0;                      /* allocation failed */
    }

    /* ---------- 1. Compute B_k(-n) for k = 0..n  ---------- */
    B[0] = (seq_t)1;
    seq_t x = -(seq_t)n; /* x = -n */

    for (idx_t m = 0; m < n; ++m) {
        /* compute S = sum_{j=0}^m binom(m,j) * B[j] */
        seq_t S = (seq_t)0;
        seq_t binom = (seq_t)1; /* binom(m,0) */

        for (idx_t j = 0; j <= m; ++j) {
            S += binom * B[(size_t)j];
            if (j < m) {
                binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);
            }
        }
        B[(size_t)(m + 1)] = x * S;            /* B_{m+1} */
    }

    /* ---------- 2. Pre‑compute powers of n ---------- */
    pow_n[0] = (seq_t)1;
    seq_t nseq = (seq_t)n;
    for (idx_t e = 1; e <= n; ++e) {
        pow_n[(size_t)e] = pow_n[(size_t)(e - 1)] * nseq;
    }

    /* ---------- 3. Compute the final sum ---------- */
    seq_t result = (seq_t)0;
    seq_t binom_nk = (seq_t)1; /* binom(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t pw_index = n - k; /* exponent of n in this term */
        seq_t term = binom_nk * B[(size_t)k] * pow_n[(size_t)pw_index];
        result += term;

        if (k < n) {
            binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    free(B);
    free(pow_n);

    return result;
}
