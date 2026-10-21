/*
 * A258832  Expansion of psi(-x^3) * f(-x, x^2) in powers of x where psi(),
 *          f(,) are Ramanujan theta functions.
 * https://oeis.org/A258832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 59, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the Euler transform of the 12‑periodic sequence 
 * c_k = {-1, 1, 0, 0, -1, -2, -1, 0, 0, 1, -1, -2} for k ≥ 1,
 * i.e. a(0)=1 and for n ≥ 1
 *      a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d * c_d ) * a(n‑k).
 */

#include <stdlib.h>

/* 12‑periodic coefficients c_k (k ≥ 1) */
static const int c[12] = { -1, 1, 0, 0, -1, -2, -1, 0, 0, 1, -1, -2 };

/* Cached values ---------------------------------------------------- */
static seq_t *values = NULL;          /* a[0..values_len-1]          */
static idx_t  values_len = 0;
static seq_t *sum_dc = NULL;          /* Σ_{d|k} d * c_d for each k   */
static idx_t  sum_dc_len = 0;

/* Expand caches up to index new_max (inclusive) */
static void extend(idx_t new_max)
{
    if (new_max < values_len)
        return; /* already sufficient */

    /* Allocate and compute sum_dc up to new_max */
    if (sum_dc) {
#ifdef __cplusplus
        delete[] sum_dc;
#else
        free(sum_dc);
#endif
    }
#ifdef __cplusplus
    sum_dc = new seq_t[new_max + 1];
#else
    sum_dc = (seq_t*)calloc((size_t)new_max + 1, sizeof(seq_t));
#endif
    sum_dc[0] = 0;
    for (idx_t d = 1; d <= new_max; ++d) {
        int cd = c[(d - 1) % 12];
        if (cd == 0) continue;
        seq_t add = (seq_t)d * (seq_t)cd;
        for (idx_t m = d; m <= new_max; m += d)
            sum_dc[m] += add;
    }
    sum_dc_len = new_max + 1;

    /* Allocate and compute values up to new_max */
    if (values) {
#ifdef __cplusplus
        delete[] values;
#else
        free(values);
#endif
    }
#ifdef __cplusplus
    values = new seq_t[new_max + 1];
#else
    values = (seq_t*)calloc((size_t)new_max + 1, sizeof(seq_t));
#endif
    values[0] = 1;
    for (idx_t n = 1; n <= new_max; ++n) {
        seq_t s = 0;
        for (idx_t k = 1; k <= n; ++k)
            s += sum_dc[k] * values[n - k];
        values[n] = s / (seq_t)n;           /* division is exact */
    }
    values_len = new_max + 1;
}

/* Sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n >= values_len)
        extend(n);
    return values[n];
}
