/*
 * A369585  Table read by rows. T(n, k) = [z^k] h(n, 1, z) where h(n, v, z)
 *          are the modified Lommel polynomials (A369117).
 * https://oeis.org/A369585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369585 – modified Lommel Table
 * T(n,k) = [z^k] 2*n*z*p(n-1,z) - p(n-2,z)
 *     p(-1,z)=0 , p(0,z)=1 .
 * a(n) is the flattened sequence of T(n,k) read by rows:
 * a(0) = 1, a(1) = 0, a(2) = 2, a(3) = -1, …
 * This implementation computes the required coefficient on the fly.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* find the row (r) and column (k) such that a(n) = T(r,k) */
    idx_t r = 0;          /* current row index */
    idx_t cum = 0;        /* cumulative count of coefficients up to previous row */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;    /* column index within row r */

    /* Base row r == 0 */
    if (r == 0) {
        return (seq_t)1;  /* only element is 1 */
    }

    /* polynomial coefficient arrays */
    seq_t *p_prevprev = NULL;   /* p_{r-2} */
    seq_t *p_prev    = NULL;    /* p_{r-1} result for previous iteration */
    seq_t *p_curr    = NULL;    /* p_{r} (current) */

    /* p_0 = [1] */
    p_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!p_prev) return (seq_t)0; /* allocation failure, though unlikely */
    p_prev[0] = (seq_t)1;

    /* iterate to compute p_1, p_2, ... , p_r */
    for (idx_t i = 1; i <= r; ++i) {
        idx_t size = i + 1; /* degree i polynomial has i+1 coefficients */
        p_curr = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!p_curr) return (seq_t)0;
        for (idx_t j = 0; j < size; ++j) p_curr[j] = (seq_t)0;

        /* add 2*i*z * p_{i-1}  --> shift left by 1 and scale by 2*i */
        seq_t mult = (seq_t)2 * (seq_t)i;
        for (idx_t j = 0; j < i; ++j) {
            p_curr[j + 1] += mult * p_prev[j];
        }

        /* subtract p_{i-2} if it exists */
        if (p_prevprev != NULL) {
            for (idx_t j = 0; j + 0 < i - 1; ++j) {
                p_curr[j] -= p_prevprev[j];
            }
        }

        /* If we've reached the required row, return the coefficient */
        if (i == r) {
            seq_t result = p_curr[k];
            /* free allocated memory */
            free(p_curr);
            free(p_prev);
            free(p_prevprev);
            return result;
        }

        /* prepare for next iteration */
        free(p_prevprev);
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    /* should never reach here */
    return (seq_t)0;
}
