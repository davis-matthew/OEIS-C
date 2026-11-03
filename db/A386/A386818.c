/*
 * A386818  Coefficients in q-expansion of E_2^2 * E_6^2, where E_2 and E_6
 *          are respectively the Eisenstein series A006352 and A013973.
 * https://oeis.org/A386818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of qⁿ in (E₂)²·(E₆)² where
 *            E₂(q)=1-24∑σ₁(k)qᵏ, E₆(q)=1-504∑σ₅(k)qᵏ. The implementation
 *            computes divisor sums σ₁ and σ₅, then the convolution of their
 *            linear combination, and finally squares that result to obtain
 *            a(n)
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate arrays for divisor sums and intermediate coefficients */
    size_t N = (size_t)n + 1;
    seq_t *sigma1 = mem_new(seq_t, N);
    seq_t *sigma5 = mem_new(seq_t, N);
    if (!sigma1 || !sigma5) return 0;                 /* sanity check */

    /* Compute σ₁ and σ₅ by a divisor‑sum sieve */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t dseq = (seq_t)d;
        /* σ₁ contribution: add d to each multiple of d */
        for (idx_t m = d; m <= n; m += d)
            sigma1[(size_t)m] += dseq;
        /* σ₅ contribution: add d⁵ to each multiple of d */
        seq_t dpow = dseq * dseq * dseq * dseq * dseq;   /* d⁵ */
        for (idx_t m = d; m <= n; m += d)
            sigma5[(size_t)m] += dpow;
    }

    /* Compute the coefficients of E₂·E₆: F[n] */
    seq_t *F = mem_new(seq_t, N);
    if (!F) { mem_free(sigma1); mem_free(sigma5); return 0; }
    F[0] = (seq_t)1;
    const seq_t C24  = (seq_t)24;
    const seq_t C504 = (seq_t)504;
    const seq_t C12096 = (seq_t)(24 * 504);           /* 12096 */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t prodSum = 0;
        for (idx_t i = 1; i < m; ++i)
            prodSum += sigma1[(size_t)i] * sigma5[(size_t)(m - i)];
        seq_t term = -C24 * sigma1[(size_t)m];
        term += -C504 * sigma5[(size_t)m];
        term += C12096 * prodSum;
        F[(size_t)m] = term;
    }

    /* Compute (E₂·E₆)²: convolution of F with itself */
    seq_t *A = mem_new(seq_t, N);
    if (!A) { mem_free(sigma1); mem_free(sigma5); mem_free(F); return 0; }

    for (idx_t i = 0; i <= n; ++i) {
        seq_t fi = F[(size_t)i];
        if (fi == (seq_t)0) continue;
        for (idx_t j = 0; j <= n - i; ++j) {
            A[(size_t)(i + j)] += fi * F[(size_t)j];
        }
    }

    seq_t result = A[(size_t)n];

    /* Free all auxiliary memory */
    mem_free(sigma1);
    mem_free(sigma5);
    mem_free(F);
    mem_free(A);

    return result;
}
