/*
 * A375062  Expansion of 1 / Sum_{k in Z} x^k / (1 - x^(5*k+1)).
 * https://oeis.org/A375062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A375062
 *  Coefficient of x^n in
 *   ∏_{k>0} ((1 - x^{5k-1}) (1 - x^{5k-4}))^3 /
 *           ((1 - x^k) (1 - x^{5k}))
 *  Implementation uses successive convolution with the factors.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static void compute_up_to(idx_t limit, seq_t **out)
{
    /* allocate two working arrays */
    seq_t *A = mem_new(seq_t, (limit + 1));
    seq_t *B = mem_new(seq_t, (limit + 1));

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i) {
        A[i] = 0;
        B[i] = 0;
    }
    A[0] = 1;

    for (idx_t k = 1; k <= limit; ++k) {
        /* multiply by 1/(1 - x^k) */
        for (idx_t j = k; j <= limit; ++j)
            A[j] += A[j - k];

        /* multiply by 1/(1 - x^{5k}) if it affects the range */
        idx_t step = 5 * k;
        if (step <= limit) {
            for (idx_t j = step; j <= limit; ++j)
                A[j] += A[j - step];
        }

        /* numerator factor (1 - x^{5k-1})^3  */
        idx_t e1 = 5 * k - 1;
        if (e1 <= limit) {
            for (idx_t i = 0; i <= limit; ++i) B[i] = 0;
            for (idx_t j = 0; j <= limit; ++j) {
                seq_t v = A[j];
                if (v == 0) continue;
                B[j] += v;
                idx_t j1 = j + e1;
                if (j1 <= limit) B[j1] -= (seq_t)3 * v;
                idx_t j2 = j + 2 * e1;
                if (j2 <= limit) B[j2] += (seq_t)3 * v;
                idx_t j3 = j + 3 * e1;
                if (j3 <= limit) B[j3] -= v;
            }
            seq_t *tmp = A; A = B; B = tmp;
        }

        /* numerator factor (1 - x^{5k-4})^3  */
        idx_t e2 = 5 * k - 4;
        if (e2 <= limit) {
            for (idx_t i = 0; i <= limit; ++i) B[i] = 0;
            for (idx_t j = 0; j <= limit; ++j) {
                seq_t v = A[j];
                if (v == 0) continue;
                B[j] += v;
                idx_t j1 = j + e2;
                if (j1 <= limit) B[j1] -= (seq_t)3 * v;
                idx_t j2 = j + 2 * e2;
                if (j2 <= limit) B[j2] += (seq_t)3 * v;
                idx_t j3 = j + 3 * e2;
                if (j3 <= limit) B[j3] -= v;
            }
            seq_t *tmp = A; A = B; B = tmp;
        }
    }

    /* forward result; free unused buffer */
    *out = A;
    mem_free(B);
}

static seq_t *coeff_cache = NULL;
static idx_t coeff_cache_len = 0;

seq_t a(idx_t n)
{
    if (!coeff_cache || n >= coeff_cache_len) {
        seq_t *new_coeff;
        compute_up_to(n, &new_coeff);
        if (coeff_cache) mem_free(coeff_cache);
        coeff_cache = new_coeff;
        coeff_cache_len = n + 1;
    }
    return coeff_cache[n];
}
