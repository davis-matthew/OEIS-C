/*
 * A291169  Expansion of 1/(1 + x/(1 + x^8/(1 + x^27/(1 + x^64/(1 + x^125/(1 +
 *          ... + x^(k^3)/(1 + ...))))))), a continued fraction.
 * https://oeis.org/A291169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291169
    a(n) is the coefficient of x^n in the continued fraction
    1/(1 + x/(1 + x^8/(1 + x^27/(1 + x^64/(1 + x^125/(1 + ... + x^(k^3)/(1 + ...)))))))
    The sequence starts: 1, -1, 1, -1, 1, -1, 1, -1, 1, 0, -1, 2, -3, 4, -5, 6, -7, 7, -6, 4, -1, -3, 8, -14, ...
*/

#include <stdlib.h>

/* Computes floor(cuberoot(n)) for unsigned 64‑bit n */
static idx_t floor_cuberoot(idx_t n)
{
    idx_t k = 0;
    while ((k + 1) * (k + 1) * (k + 1) <= n)
        ++k;
    return k;
}

/* Cached data for computed sequence, coefficients of numerator and denominator */
/* All arrays are of length cached_len == current maximum n + 1 */
static seq_t *cached_numerator = NULL;
static seq_t *cached_denominator = NULL;
static seq_t *cached_coeffs = NULL;
static idx_t cached_len = 0;

/* Recomputes values up to new_len-1 (new_len = n+1) */
static void recompute(idx_t n)
{
    size_t new_len = (size_t)(n + 1);
    if (cached_len >= new_len)
        return;                                 /* nothing to do */

    /* Allocate temporary arrays */
    seq_t *n_cur   = (seq_t *)calloc(new_len, sizeof(seq_t));
    seq_t *n_next  = (seq_t *)calloc(new_len, sizeof(seq_t));
    seq_t *d_cur   = (seq_t *)calloc(new_len, sizeof(seq_t));
    if (!n_cur || !n_next || !d_cur) {
        /* memory error – terminate */
        abort();
    }

    /* Init base: G_{K+1} = 1  ->  n_cur = 1, d_cur = 1 */
    n_cur[0] = 1;
    d_cur[0] = 1;

    /* Build polynomials from k = floor_cuberoot(n) down to 1 */
    size_t k = floor_cuberoot(n);
    while (k >= 1) {
        size_t shift = (size_t)k * k * k;       /* k^3 */

        /* n_next = n_cur + shift(d_cur) */
        for (size_t i = 0; i < new_len; ++i) {
            seq_t val = n_cur[i];
            if (i >= shift)
                val += d_cur[i - shift];
            n_next[i] = val;
        }

        /* Prepare for next iteration:
           new denominator d_cur = old n_cur,
           new numerator  n_cur  = n_next */
        seq_t *tmp = d_cur;
        d_cur = n_cur;
        n_cur = n_next;
        n_next = tmp;

        if (k == 1)        /* avoid underflow of unsigned k */
            break;
        --k;
    }

    /* At this point:
       - n_cur   holds denominator polynomial n_1
       - d_cur   holds numerator    polynomial d_1   (which equals n_2)
    */

    /* Allocate array for sequence coefficients */
    seq_t *new_coeffs = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_coeffs) {
        free(n_cur);
        free(d_cur);
        abort();
    }

    /* Compute quotient d_cur / n_cur up to degree n */
    new_coeffs[0] = 1;                           /* because d_cur[0]/n_cur[0] = 1 */
    for (size_t m = 1; m < new_len; ++m) {
        seq_t sum = d_cur[m];
        for (size_t i = 1; i <= m; ++i)
            sum -= n_cur[i] * new_coeffs[m - i];
        new_coeffs[m] = sum;
    }

    /* Free old cached data */
    free(cached_numerator);
    free(cached_denominator);
    free(cached_coeffs);

    /* Store new data */
    cached_numerator   = d_cur;
    cached_denominator = n_cur;
    cached_coeffs      = new_coeffs;
    cached_len         = new_len;

    /* free temporary buffer (now empty) */
    free(n_next);
}

/* Public sequence function
   a(n) = coefficient of x^n in the continued fraction expansion */
seq_t a(idx_t n)
{
    if (n + 1 > cached_len)
        recompute(n);

    return cached_coeffs[(size_t)n];
}
