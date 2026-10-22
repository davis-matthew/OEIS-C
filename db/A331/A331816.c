/*
 * A331816  Irregular triangle (read by rows) of coefficients T(n,k) of
 *          polynomials p(n,x) = Sum_{k=0..2*n} T(n,k) * x^k = (-1)^n *
 *          e^(x^3/3) * (((d/dx)^n) e^(-x^3/3)) for n >= 0 and 0 <= k <= 2*n.
 * https://oeis.org/A331816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 38, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A331816 – values of the irregular triangle T(n,k)
 * computed row‑by‑row using recurrence 
 *   T(n,k) = T(n‑1,k‑2) – (k+1) * T(n‑1,k+1)
 * with T(0,0)=1 and T(n,k)=0 outside 0≤k≤2n.
 * The sequence a(n) is obtained by reading the triangle by rows: 
 * a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(1,2), a(4)=T(2,0), ...
 *       a(n) = T( floor(√n), n – floor(√n)² ).
 */

#include <stdlib.h>
#include <math.h>

/* ---------- Helpers for dynamic caching ---------- */
static seq_t **rows = NULL;          /* rows[i] holds T(i,0..2*i) */
static size_t  row_count = 0;        /* number of rows presently stored */

/* Ensure that rows[0..n] are computed and cached */
static void ensure_row(size_t n)
{
    /* Allocate base row 0 if needed */
    if (row_count == 0) {
        rows = (seq_t**)malloc(sizeof(seq_t*));
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        rows[0][0] = (seq_t)1;        /* T(0,0) = 1 */
        row_count = 1;
    }
    /* Compute any missing rows up to n */
    while (row_count <= n) {
        size_t i = row_count;          /* index of the new row */
        size_t len = 2*i + 1;          /* number of columns in row i */
        /* Grow the array of row pointers */
        rows = (seq_t**)realloc(rows, (i+1) * sizeof(seq_t*));
        /* Allocate and zero initialize the new row */
        rows[i] = (seq_t*)calloc(len, sizeof(seq_t));
        /* Get previous row data */
        seq_t *prev   = rows[i-1];
        size_t prelen = 2*(i-1) + 1;
        /* Fill the new row using the recurrence */
        for (size_t k = 0; k < len; ++k) {
            seq_t a = 0, b = 0;                          /* T(i-1,k-2), T(i-1,k+1) */
            if (k >= 2 && (k-2) < prelen) a = prev[k-2];
            if ((k+1) < prelen)                     b = prev[k+1];
            rows[i][k] = a - ((seq_t)(k+1) * b);
        }
        row_count = i + 1;
    }
}

/* Return the requested triangle entry T(r,k) */
static seq_t get_T(size_t r, size_t k)
{
    ensure_row(r);
    return rows[r][k];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the row r = floor(sqrt(n)) */
    idx_t r = (idx_t)sqrt((double)n);
    while ((r+1)*(r+1) <= n) ++r;
    while (r*r > n) --r;
    /* Compute offset within the row: k = n - r² */
    idx_t k = n - r*r;
    /* Return the corresponding triangle entry */
    return get_T((size_t)r, (size_t)k);
}
