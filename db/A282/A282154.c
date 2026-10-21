/*
 * A282154  Coefficients in expansion of Eisenstein series
 *          -q*(d/dq)(q*(d/dq)E_2).
 * https://oeis.org/A282154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -24*(σ₁σ₁)(n) + 192*(σ₁σ₁σ₁)(n) + 10*σ₃(n) -
 *            240*(σ₁σ₃)(n) + 14*σ₅(n) where σ_k(n) = Σ_{d|n} d^k and (f·g)(n)
 *            = Σ_{i+j=n} f(i)·g(j) is additive convolution
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 407, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */

    size_t N = (size_t)n;

    /* Allocate arrays for σ₁, σ₃, σ₅ */
    seq_t *sigma1 = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *sigma3 = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *sigma5 = (seq_t *)calloc(N + 1, sizeof(seq_t));

    /* Compute divisor sums */
    for (size_t d = 1; d <= N; ++d) {
        seq_t d1 = (seq_t)d;
        seq_t d2 = d1 * d1;
        seq_t d3 = d2 * d1;
        seq_t d5 = d3 * d2;  /* d^5 = d^3 * d^2 */

        for (size_t m = d; m <= N; m += d) {
            sigma1[m] += d1;
            sigma3[m] += d3;
            sigma5[m] += d5;
        }
    }

    /* Additive convolution arrays: (σ₁·σ₁), (σ₁·σ₃), and (σ₁·σ₁·σ₁) */
    seq_t *s2   = (seq_t *)calloc(N + 1, sizeof(seq_t));  /* σ₁·σ₁ */
    seq_t *s13  = (seq_t *)calloc(N + 1, sizeof(seq_t));  /* σ₁·σ₃ */
    seq_t *s3   = (seq_t *)calloc(N + 1, sizeof(seq_t));  /* σ₁·σ₁·σ₁ */

    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = 1; i + j <= N; ++j) {
            seq_t ij = sigma1[i] * sigma1[j];
            s2[i + j] += ij;

            seq_t ij13 = sigma1[i] * sigma3[j];
            s13[i + j] += ij13;
        }
    }

    /* s3 = s2 · σ₁ */
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = 1; i + j <= N; ++j) {
            seq_t val = s2[i] * sigma1[j];
            s3[i + j] += val;
        }
    }

    /* Compute a(n) with the derived formula */
    seq_t term_pos = (seq_t)192 * s3[n] + (seq_t)10 * sigma3[n] + (seq_t)14 * sigma5[n];
    seq_t term_neg = (seq_t)24  * s2[n] + (seq_t)240 * s13[n];
    seq_t result   = term_pos - term_neg;   /* the combination is guaranteed non‑negative */

    /* Free temporary allocations */
    free(sigma1);
    free(sigma3);
    free(sigma5);
    free(s2);
    free(s13);
    free(s3);

    return result;
}
