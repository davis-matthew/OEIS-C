/*
 * A256637  Expansion of psi(-q) * phi(-q^3)^2 / (q * psi(-q^3)^3) in powers
 *          of q where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A256637
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^{n+1} in
 *            eta(q)*eta(q^3)*eta(q^4)*eta(q^6) / divided by
 *            eta(q^2)*eta(q^12)^3
 * Range:     i8 n <= 65, i16 n <= 199, i32 n <= 653, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   free_seq_arr, pentagonal_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/free_seq_arr.h"
#include "../utils/pentagonal_series.h"

/* seq_t and idx_t are defined by the build. */

/* Allocate and free an array of seq_t. Works in C and C++. */
static seq_t *alloc_seq_arr(size_t len) {
#if defined(__cplusplus)
    seq_t *p = new seq_t[len];
    for (size_t i = 0; i < len; ++i) p[i] = (seq_t)0;
    return p;
#else
    seq_t *p = (seq_t *)malloc(len * sizeof(seq_t));
    if (!p) abort();
    for (size_t i = 0; i < len; ++i) p[i] = (seq_t)0;
    return p;
#endif
}

/* Multiply two series a and b truncated to length L. */
static seq_t *multiply_series(const seq_t *a, size_t len_a,
                              const seq_t *b, size_t len_b,
                              size_t L) {
    seq_t *res = alloc_seq_arr(L);
    for (size_t i = 0; i < len_a && i < L; ++i) {
        if (a[i] == (seq_t)0) continue;
        for (size_t j = 0; j < len_b && i + j < L; ++j) {
            if (b[j] == (seq_t)0) continue;
            res[i + j] += a[i] * b[j];
        }
    }
    return res;
}

/* Compute the series S(q) up to length len (indices 0..len-1). */
static void compute_series(size_t len, seq_t **C_out) {
    /* Build each eta factor's (1 − q^{m k}) products. */
    seq_t *S1  = alloc_seq_arr(len);  pentagonal_series( 1, len, S1);
    seq_t *S3  = alloc_seq_arr(len);  pentagonal_series( 3, len, S3);
    seq_t *S4  = alloc_seq_arr(len);  pentagonal_series( 4, len, S4);
    seq_t *S6  = alloc_seq_arr(len);  pentagonal_series( 6, len, S6);
    seq_t *S2  = alloc_seq_arr(len);  pentagonal_series( 2, len, S2);
    seq_t *S12 = alloc_seq_arr(len);  pentagonal_series(12, len, S12);

    /* Numerator: S1 * S3 * S4 * S6 */
    seq_t *temp1 = multiply_series(S1, len, S3, len, len);
    seq_t *temp2 = multiply_series(temp1, len, S4, len, len);
    seq_t *N     = multiply_series(temp2, len, S6, len, len);

    /* Denominator: S2 * S12^3 */
    seq_t *S12_2 = multiply_series(S12, len, S12, len, len);
    seq_t *S12_3 = multiply_series(S12_2, len, S12, len, len);
    seq_t *D     = multiply_series(S2,   len, S12_3, len, len);

    /* Free intermediate arrays. */
    free_seq_arr(S1);  free_seq_arr(S3);  free_seq_arr(S4);
    free_seq_arr(S6);  free_seq_arr(S2);  free_seq_arr(S12);
    free_seq_arr(temp1); free_seq_arr(temp2);
    free_seq_arr(S12_2); free_seq_arr(S12_3);

    /* Perform series division: C = N / D.  D[0] == 1. */
    seq_t *C = alloc_seq_arr(len);
    C[0] = N[0];
    for (size_t m = 1; m < len; ++m) {
        seq_t sum = N[m];
        for (size_t k = 1; k <= m; ++k) {
            sum -= D[k] * C[m - k];
        }
        C[m] = sum;
    }

    /* Free N and D. */
    free_seq_arr(N); free_seq_arr(D);

    *C_out = C;
}

/* Memoized series storage. */
static seq_t *C_series = NULL;
static size_t C_len   = 0;

seq_t a(idx_t n) {
    if (n < -1) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* We need C[0..n+1] to return a(n) = C[n+1]. */
    size_t needed_len = (size_t)(n + 2);
    if (needed_len > C_len) {
        /* recompute entire series up to needed_len */
        if (C_series) free_seq_arr(C_series);
        compute_series(needed_len, &C_series);
        C_len = needed_len;
    }
    return C_series[(size_t)(n + 1)];
}
