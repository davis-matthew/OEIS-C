/*
 * A282000  Coefficients in q-expansion of E_4^3*E_6, where E_4 and E_6 are
 *          respectively the Eisenstein series A004009 and A013973.
 * https://oeis.org/A282000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0: a(n) = 3*d*σ₃(n) + 3*d²*(σ₃*σ₃)(n) +
 *            d³*(σ₃*σ₃*σ₃)(n) - 504*∑_{k=1..n} σ₅(k) * a(n-k), where d = 240
 *            and σₖ(m) = ∑_{d|m} dᵏ
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 13, i128 n <= 181,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the coefficients of E₄³·E₆ directly by convolution. */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* Utilities from ../utils/ */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Allocate arrays of length n+1. */
    size_t N = (size_t)n + 1u;

    seq_t *sigma3 = alloc_seq_array(N);
    seq_t *sigma5 = alloc_seq_array(N);
    seq_t *S2     = alloc_seq_array(N);
    seq_t *S3     = alloc_seq_array(N);
    seq_t *c      = alloc_seq_array(N);

    /* --- Compute σ₃ and σ₅ --- */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t i2 = ii * ii;
        seq_t i3 = ii * i2;     /* i³ */
        seq_t i5 = i3 * i2;     /* i⁵ */
        for (idx_t j = i; j <= n; j += i) {
            sigma3[j] += i3;
            sigma5[j] += i5;
        }
    }

    /* --- Convolution σ₃ * σ₃ ⇒ S2 --- */
    for (idx_t t = 1; t <= n; ++t) {
        seq_t sum = 0;
        for (idx_t i = 1; i < t; ++i) {
            sum += sigma3[i] * sigma3[t - i];
        }
        S2[t] = sum;
    }

    /* --- Convolution S2 * σ₃ ⇒ S3 --- */
    for (idx_t t = 1; t <= n; ++t) {
        seq_t sum = 0;
        for (idx_t i = 1; i < t; ++i) {
            sum += sigma3[i] * S2[t - i];
        }
        S3[t] = sum;
    }

    /* --- Compute c[n] = coefficient of E₄³ --- */
    seq_t d  = 240;
    seq_t d2 = d * d;
    seq_t d3 = d2 * d;

    c[0] = 1;
    for (idx_t t = 1; t <= n; ++t) {
        seq_t term1 = (seq_t)3 * d * sigma3[t];
        seq_t term2 = (seq_t)3 * d2 * S2[t];
        seq_t term3 = d3 * S3[t];
        c[t] = term1 + term2 + term3;
    }

    /* --- Compute a[n] = c[n] - 504 * (σ₅ * c)[n] --- */
    seq_t e = 504;
    seq_t result = c[n];
    for (idx_t k = 1; k <= n; ++k) {
        result -= e * (sigma5[k] * c[n - k]);
    }

    /* Free temporary arrays */
    free_seq_array(sigma3);
    free_seq_array(sigma5);
    free_seq_array(S2);
    free_seq_array(S3);
    free_seq_array(c);

    return result;
}
