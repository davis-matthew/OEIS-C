/*
 * A249100  Triangular array read by rows: row n gives the coefficients of the
 *          polynomial p(n,x) defined in Comments.
 * https://oeis.org/A249100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 135, u64 n <= 378,
 *            u128 n <= 1129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = T(n-1,k-1) + (2*n+1)*T(n-2,k)
   initial rows: T(0,0) = 1
                 T(1,0) = 3, T(1,1) = 1
   a(n) is the row‑by‑row flattening of these coefficients. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    idx_t k = n - (r * (r + 1)) / 2;

    /* Static storage for the triangle rows.  */
    static seq_t **rows = NULL;   /* array of pointers to each row */
    static idx_t rows_cap = 0;    /* capacity of rows array       */
    static idx_t rows_used = 0;   /* number of rows allocated     */

    /* Grow the rows array if needed and compute missing rows. */
    if (r >= rows_used) {
        /* Resize rows array. */
        if (rows_cap == 0) rows_cap = 4;
        while (rows_cap <= r) rows_cap <<= 1;

        seq_t **tmp = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
        if (!tmp) exit(1);
        rows = tmp;
        for (idx_t i = rows_used; i < rows_cap; ++i) rows[i] = NULL;

        /* Compute new rows. */
        for (idx_t i = rows_used; i <= r; ++i) {
            seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            if (!row) exit(1);

            /* Initialise to zero. */
            for (idx_t j = 0; j <= i; ++j) row[j] = (seq_t)0;

            if (i == 0) {
                row[0] = (seq_t)1;
            } else if (i == 1) {
                row[0] = (seq_t)3;
                row[1] = (seq_t)1;
            } else {
                idx_t prev  = i - 1;
                idx_t prev2 = i - 2;
                for (idx_t col = 0; col <= i; ++col) {
                    seq_t term1 = (col > 0) ? rows[prev][col - 1] : (seq_t)0;
                    seq_t term2 = 0;
                    if (col <= prev2) {
                        term2 = rows[prev2][col] * (seq_t)(2 * (seq_t)i + 1);
                    }
                    row[col] = term1 + term2;
                }
            }
            rows[i] = row;
        }
        rows_used = r + 1;
    }

    return rows[r][k];
}
