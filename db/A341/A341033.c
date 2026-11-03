/*
 * A341033  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(x/(1-k*x)).
 * https://oeis.org/A341033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term read along antidiagonals of the square * array
 *            T(n,k) defined by * T(0,k) = 1, * T(1,k) = 1, * T(n,k) =
 *            (2*k*n-2*k+1)*T(n-1,k) - k*k*(n-1)*(n-2)*T(n-2,k) for n>1. * The
 *            function below maps the 1‑dimensional index n to the coordinates
 *            * (row, col) of that antidiagonal and evaluate...
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 86, u64 n <= 184,
 *            u128 n <= 429, big any n
 * Validated: standard, pass, 71 terms through n = 7955 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, but idx_t is defined elsewhere */

static seq_t
T_row_col(idx_t row, idx_t col)
{
    if (row == 0)
        return (seq_t)1;
    if (row == 1)
        return (seq_t)1;

    seq_t tprev2 = (seq_t)1; /* T(0,k) */
    seq_t tprev1 = (seq_t)1; /* T(1,k) */
    seq_t tcur;

    for (idx_t m = 2; m <= row; ++m) {
        /* term1 = (2*k*m-2*k+1) * T(m-1,k) */
        seq_t coeff1 = ((seq_t)2 * (seq_t)col * (seq_t)m
                        - (seq_t)2 * (seq_t)col + (seq_t)1);
        seq_t term1  = coeff1 * tprev1;

        /* term2 = k^2 * (m-1)*(m-2) * T(m-2,k) */
        seq_t coeff2 = (seq_t)col * (seq_t)col
                       * (seq_t)(m - 1) * (seq_t)(m - 2);
        seq_t term2  = coeff2 * tprev2;

        tcur = term1 - term2;

        tprev2 = tprev1;
        tprev1 = tcur;
    }
    return tprev1;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that
     *   d*(d+1)/2 <= n < (d+1)*(d+2)/2
     */
    idx_t d = 0;
    idx_t limit = 1;               /* (d+1)*(d+2)/2 for current d */
    while (limit <= n) {
        ++d;
        limit = (d + 1) * (d + 2) / 2;
    }

    idx_t start   = d * (d + 1) / 2;   /* first index of diagonal d */
    idx_t offset  = n - start;        /* position inside diagonal */
    idx_t row = offset;               /* i in (i,j) */
    idx_t col = d - offset;           /* j = d - i */

    return T_row_col(row, col);
}
