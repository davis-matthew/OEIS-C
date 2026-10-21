/*
 * A257624  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = 3*x + 5.
 * https://oeis.org/A257624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2, and T
 *            satisfies T(0,0) = 1, T(n,k) = (3*k+5)·T(n-1,k) +
 *            (3*(n-k)+5)·T(n-1,k-1), with T(n,0) = 5·T(n-1,0)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 38, u64 n <= 110, u128 n <= 334,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

/* static storage for the computed rows of the triangle                     */
static seq_t **row_vals   = NULL;  /* pointers to rows, row_vals[r][k]    */
static idx_t   rows_cap   = 0;     /* capacity of row_vals array          */
static idx_t   max_row    = 0;     /* largest row index currently computed*/
static idx_t   total_terms = 0;    /* total number of terms computed      */

/* --------------------------------------------------------------------- */
/* Compute rows of the triangle up to row r (inclusive).                 */
static void ensure_rows_upto(idx_t r)
{
    if (row_vals == NULL) {                       /* first call          */
        rows_cap  = r + 1;
        row_vals  = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        if (row_vals == NULL)
            return;                               /* allocation failure  */
        /* row 0 */
        row_vals[0] = (seq_t*)malloc(sizeof(seq_t));
        if (row_vals[0] == NULL)
            return;                               /* allocation failure  */
        row_vals[0][0]   = (seq_t)1;
        max_row         = 0;
        total_terms     = 1;
    }

    /* compute any missing higher rows */
    while (max_row < r) {
        idx_t      next   = max_row + 1;    /* row index to compute   */
        idx_t      len    = next + 1;       /* number of entries     */
        seq_t     *prev   = row_vals[max_row];
        seq_t     *cur    = (seq_t*)malloc(len * sizeof(seq_t));
        if (cur == NULL)
            return;                         /* allocation failure    */

        /* compute the current row */
        if (next == 0) {                     /* never taken here      */
            cur[0] = (seq_t)1;
        } else {
            for (idx_t k = 0; k <= next; ++k) {
                if (k == 0) {
                    /* T(next,0) = 5 * T(next-1,0) */
                    cur[0] = (seq_t)5 * prev[0];
                } else {
                    seq_t term1 = 0;
                    if (k <= next-1)               /* T(n-1,k) exists   */
                        term1 = (seq_t)(3*k + 5) * prev[k];
                    /* T(n-1,k-1) always valid for k>=1 */
                    seq_t term2 = (seq_t)(3*(next - k) + 5) * prev[k-1];
                    cur[k] = term1 + term2;
                }
            }
        }

        /* store the new row, growing row_vals if needed */
        if (next >= rows_cap) {
            rows_cap = next + 1;
            seq_t **tmp = (seq_t**)realloc(row_vals, rows_cap * sizeof(seq_t*));
            if (tmp == NULL) {
                free(cur);
                return;                     /* reallocation failure*/
            }
            row_vals = tmp;
        }
        row_vals[next] = cur;
        max_row        = next;
        total_terms   += len;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
seq_t a(idx_t n)
{
    /* find the row r that contains the n‑th term                */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* correct any rounding error                                */
    while (r*(r+1)/2 > n) --r;
    while ((r+1)*(r+2)/2 <= n) ++r;

    /* column index within that row                               */
    idx_t before = r * (r + 1) / 2;          /* terms before row r   */
    idx_t k      = n - before;              /* k is in [0, r]      */

    /* compute rows up to r if needed                             */
    ensure_rows_upto(r);

    return row_vals[r][k];
}
