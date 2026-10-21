/*
 * A249758  Triangular array of coefficients of polynomials p(n,x) = (x +
 *          2)*p(n-1,x) + x, p(0,x) = 1.
 * https://oeis.org/A249758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 69, u32 n <= 257, u64 n <= 915,
 *            u128 n <= 3509, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A249758.
    Sequence: a(n) is the (row,position)-ordered coefficients of the
    polynomials p(n,x) defined by
        p(0,x) = 1
        p(n,x) = (x + 2) * p(n-1,x) + x
    Example rows:
        p(0,x) = 1
        p(1,x) = 2 + 2*x
        p(2,x) = 4 + 7*x + 2*x^2
        p(3,x) = 8 + 19*x + 11*x^2 + 2*x^3
    The function a(n) returns the n-th term in the sequence listed
    row after row, left to right.
*/

#include <stdlib.h>

/*------------------------------------------------------------*/
/*  Static state – cache of all computed rows.                */
/*------------------------------------------------------------*/
static seq_t **rows = NULL;   /* array of pointers to rows */
static size_t rows_cap = 0;   /* allocated slots for rows      */
static size_t rows_cnt = 0;   /* number of rows stored         */

/*------------------------------------------------------------*/
/*  Ensure that rows[0 ... r] exist.  Computes any missing   */
/*  rows and appends them to the cache.                       */
/*------------------------------------------------------------*/
static void ensure_row(idx_t r)
{
    /* Helper to grow the rows pointer array. */
    if (r >= rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 1;
        while (new_cap <= r) new_cap *= 2;
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows_cap = new_cap;
    }

    while (rows_cnt <= r) {
        if (rows_cnt == 0) {
            /* Row 0: single coefficient 1. */
            seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
            row0[0] = (seq_t)1;
            rows[0] = row0;
            rows_cnt = 1;
        } else {
            /* Compute row r from row r-1. */
            idx_t prev_r = rows_cnt - 1;      /* index of previous row   */
            seq_t *prev    = rows[prev_r];    /* pointer to previous row */
            idx_t new_r    = rows_cnt;        /* index of new row (r)    */
            size_t new_len = new_r + 1;       /* number of coefficients  */
            seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));

            /* First coefficient (degree 0). */
            new_row[0] = (seq_t)2 * prev[0];

            if (new_r == 1) {
                /* Row 1: special treatment – last coefficient gets +1. */
                new_row[1] = prev[0] + (seq_t)1;
            } else {
                /* Intermediate coefficients 1 .. new_r-1. */
                for (idx_t k = 1; k < new_r; ++k) {
                    new_row[k] = (seq_t)2 * prev[k] + prev[k-1];
                    if (k == 1) new_row[k] += (seq_t)1;
                }
                /* Last coefficient. */
                new_row[new_r] = prev[new_r-1];
            }

            rows[rows_cnt] = new_row;
            ++rows_cnt;
        }
    }
}

/*------------------------------------------------------------*/
/*  Find the row index that contains term n.                 */
/*  The number of terms up to row r is T(r) = (r+1)(r+2)/2.   */
/*  We need the smallest r with T(r) > n.                      */
/*------------------------------------------------------------*/
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (( (idx_t)(r + 1) * (idx_t)(r + 2) ) / 2 <= n) {
        ++r;
    }
    return r;
}

/*------------------------------------------------------------*/
/*  Public entry point.  Return a(n).                         */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Locate the row and position within that row. */
    idx_t row = row_of(n);
    idx_t terms_before = (idx_t)row * (row + 1) / 2;   /* T(row-1)   */
    idx_t pos  = n - terms_before;                    /* 0‑based pos */

    /* Make sure all needed rows are computed. */
    ensure_row(row);

    /* Return the requested coefficient. */
    return rows[row][pos];
}
