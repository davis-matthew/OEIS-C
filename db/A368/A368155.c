/*
 * A368155  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 3*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - 3*x - 2*x^2.
 * https://oeis.org/A368155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  The sequence is obtained by reading row‑by‑row the
 * coefficients of the polynomials p(n,x) defined by:
 *   p(1,x) = 1
 *   p(2,x) = 1 + 3*x
 *   p(n,x) = (1+3*x)*p(n-1,x) + (1-3*x-2*x^2)*p(n-2,x)   (n≥3)
 * The n‑th term a(n) is the coefficient of x^{c-1} in p(r,x),
 * where r is the unique integer with r(r+1)/2 ≥ n,
 * and c = n - (r-1)r/2 (1‑based column).
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Compute a(n) for the OEIS sequence A368155.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the trivial first two rows directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* Find r such that r(r+1)/2 ≥ n,  r ≥ 1 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;

    /* Column c (1‑based) within row r */
    idx_t prev_rows_sum = (r - 1) * r / 2;   /* sum_{i=1}^{r-1} i */
    idx_t c = n - prev_rows_sum;            /* 1 ≤ c ≤ r */

    /* Allocate coefficient arrays for rows 1, 2 and iterate up to r */
    /* p(1,x) coefficients */
    seq_t *prev2 = (seq_t*)malloc(sizeof(seq_t) * 1);
    prev2[0] = (seq_t)1;

    /* p(2,x) coefficients */
    seq_t *prev1 = (seq_t*)malloc(sizeof(seq_t) * 2);
    prev1[0] = (seq_t)1;
    prev1[1] = (seq_t)3;

    /* If r == 2 we would have returned earlier, so r ≥ 3 here */
    for (idx_t i = 3; i <= r; ++i) {
        idx_t len = i;                     /* current row has i terms */
        seq_t *curr = (seq_t*)calloc(len, sizeof(seq_t));

        /* Contribution from u * p(i-1,x),  u = 1 + 3*x */
        for (idx_t j = 0; j < i - 1; ++j) {
            curr[j]     += prev1[j];                 /* *1     */
            curr[j + 1] += prev1[j] * (seq_t)3;      /* *3x    */
        }

        /* Contribution from v * p(i-2,x),  v = 1 - 3*x - 2*x^2 */
        for (idx_t j = 0; j < i - 2; ++j) {
            curr[j]     += prev2[j];                 /* *1     */
            curr[j + 1] += prev2[j] * (seq_t)(-3);   /* *(-3x) */
            curr[j + 2] += prev2[j] * (seq_t)(-2);   /* *(-2x²)*/
        }

        /* If this is the target row, grab the needed coefficient */
        if (i == r) {
            seq_t result = curr[c - 1];

            /* Free all allocated memory */
            free(prev2);
            free(prev1);
            free(curr);

            return result;
        }

        /* Roll the buffers: p(i-2) <- p(i-1), p(i-1) <- p(i) */
        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    /* Should never reach here */
    return (seq_t)0;
}
