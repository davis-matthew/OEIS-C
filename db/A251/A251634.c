/*
 * A251634  Numerators of inverse Riordan triangle of Riordan triangle
 *          A029635. Riordan (1/(1-x), x/(1+2*x)). Triangle read by rows for 0
 *          <= m <= n.
 * https://oeis.org/A251634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the n-th element (0‑based) of the triangular array
 *            T(n,k) defined by T(0,0) = 1 T(n,0) = 1 for n > 0 T(n,k) =
 *            T(n-1,k-1) - 2*T(n-1,k) for 1 ≤ k ≤ n a(n) is the flattened
 *            sequence obtained by reading the rows of T from left to right,
 *            top to bottom
 * Range:     i8 n <= 29, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <math.h>

/* Helper: convert a flattened index into (row, column). */
static inline idx_t _row_from_index(idx_t idx)
{
    double r = (sqrt((double)idx * 8.0 + 1.0) - 1.0) / 2.0;
    idx_t rr = (idx_t)r;
    /* Adjust in case of rounding errors */
    while ((rr + 1) * (rr + 2) / 2 <= idx) ++rr;
    while (rr * (rr + 1) / 2 > idx) --rr;
    return rr;
}

/* Cached triangular arrays.  tri_rows[i] points to an array
   of length i+1 that holds T(i,0..i). */
static seq_t **tri_rows = NULL;
static idx_t tri_rows_cap = 0;   /* number of rows currently stored */

/* Ensure that rows up to 'last' are computed and stored. */
static void _ensure_rows(idx_t last)
{
    if (tri_rows_cap > last) return;
    /* Allocate or grow the pointer array */
    idx_t old_cap = tri_rows_cap;
    tri_rows_cap = last + 1;
    tri_rows = (seq_t**)realloc(tri_rows, tri_rows_cap * sizeof(seq_t*));
    /* Compute missing rows, starting from old_cap */
    for (idx_t n = old_cap; n <= last; ++n)
    {
        seq_t *row = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        row[0] = (seq_t)1;                     /* T(n,0) = 1 */
        for (idx_t k = 1; k <= n; ++k)
        {
            seq_t val = tri_rows[n-1][k-1];    /* T(n-1,k-1) */
            if (k <= n-1)
                val -= (seq_t)2 * tri_rows[n-1][k];  /* -2*T(n-1,k) */
            row[k] = val;
        }
        tri_rows[n] = row;
    }
}

/* Public sequence function */
seq_t a(idx_t idx)
{
    /* Find row and column of the requested element */
    idx_t row = _row_from_index(idx);
    idx_t col = idx - row * (row + 1) / 2;

    /* Make sure the required row is available */
    _ensure_rows(row);

    return tri_rows[row][col];
}
