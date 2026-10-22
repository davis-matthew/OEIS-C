/*
 * A386813  Coefficients in q-expansion of E_2^3 * E_4^2, where E_2 and E_4
 *          are respectively the Eisenstein series A006352 and A004009.
 * https://oeis.org/A386813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in (E₂(q))³ · (E₄(q))², where E₂ = 1 –
 *            24 Σ_{k≥1} σ₁(k) qᵏ, E₄ = 1 + 240 Σ_{k≥1} σ₃(k) qᵏ
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 6, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes divisor sums σ₁ and σ₃, then
   builds the sequences a₂(n) = coeff of E₂ and a₄(n) = coeff of E₄,
   convolves to obtain a₂³ and a₄², and finally convolves those two
   sequences to produce the sought coefficients.  Results are cached
   and recomputed only when a larger index is requested. */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static idx_t current_cap = 0;
static seq_t *sigma1_arr = NULL;
static seq_t *sigma3_arr = NULL;
static seq_t *a2_arr     = NULL;
static seq_t *a4_arr     = NULL;
static seq_t *a2pow3_arr = NULL;
static seq_t *a4pow2_arr = NULL;
static seq_t *res_arr    = NULL;

/* Recompute all auxiliary arrays to cover indices [0..new_cap-1]. */
static void recompute(idx_t new_cap)
{
    idx_t N = new_cap - 1;              /* largest required index */
    idx_t doubleN = N * 2;              /* maximum index in some convolutions */
    size_t sz1 = (size_t)(N + 1);
    size_t sz2 = (size_t)(doubleN + 1);

    /* Free previously allocated memory. */
    free(sigma1_arr); free(sigma3_arr);
    free(a2_arr); free(a4_arr);
    free(a2pow3_arr); free(a4pow2_arr);
    free(res_arr);

    /* Allocate fresh arrays. */
    sigma1_arr  = alloc_zero(sz1);
    sigma3_arr  = alloc_zero(sz1);
    a2_arr      = alloc_zero(sz1);
    a4_arr      = alloc_zero(sz1);
    a2pow3_arr  = alloc_zero(sz1);
    a4pow2_arr  = alloc_zero(sz1);
    res_arr     = alloc_zero(sz1);

    /* Compute σ₁ and σ₃ divisor sums for 1..N. */
    for (idx_t d = 1; d <= N; ++d) {
        seq_t d_sq  = (seq_t)d * d;
        seq_t d_cu  = d_sq * (seq_t)d;
        for (idx_t m = d; m <= N; m += d) {
            sigma1_arr[m] += d;
            sigma3_arr[m] += d_cu;
        }
    }

    /* a₂[n] = 1 – 24 σ₁(n)   (a₂[0] = 1). */
    a2_arr[0] = 1;
    for (idx_t i = 1; i <= N; ++i) {
        a2_arr[i] = -(seq_t)24 * sigma1_arr[i];
    }

    /* a₄[n] = 1 + 240 σ₃(n)  (a₄[0] = 1). */
    a4_arr[0] = 1;
    for (idx_t i = 1; i <= N; ++i) {
        a4_arr[i] = (seq_t)240 * sigma3_arr[i];
    }

    /* a₂³ = a₂ * a₂ * a₂   (convolution twice, limited to N). */
    /* First compute a₂² into a temporary buffer c2. */
    seq_t *c2 = alloc_zero(sz2);
    for (idx_t i = 0; i <= N; ++i) {
        for (idx_t j = 0; j <= N - i; ++j) {
            c2[i + j] += a2_arr[i] * a2_arr[j];
        }
    }
    for (idx_t i = 0; i <= N; ++i) {
        for (idx_t j = 0; j <= N - i; ++j) {
            a2pow3_arr[i + j] += c2[i] * a2_arr[j];
        }
    }
    free(c2);

    /* a₄² = a₄ * a₄   (single convolution, limited to N). */
    for (idx_t i = 0; i <= N; ++i) {
        for (idx_t j = 0; j <= N - i; ++j) {
            a4pow2_arr[i + j] += a4_arr[i] * a4_arr[j];
        }
    }

    /* Final convolution: result[n] = Σ_{i+j=n} (a₂³)[i] · (a₄²)[j]. */
    for (idx_t i = 0; i <= N; ++i) {
        for (idx_t j = 0; j <= N - i; ++j) {
            res_arr[i + j] += a2pow3_arr[i] * a4pow2_arr[j];
        }
    }

    current_cap = new_cap;
}

/* Public function as required. */
seq_t a(idx_t n)
{
    if (n >= current_cap) {
        recompute(n + 1);
    }
    return res_arr[n];
}
