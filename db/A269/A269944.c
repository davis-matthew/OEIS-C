/*
 * A269944  Triangle read by rows, Stirling cycle numbers of order 2, T(n, n)
 *          = 1, T(n, k) = 0 if k < 0 or k > n, otherwise T(n, k) = T(n-1,
 *          k-1) + (n-1)^2*T(n-1, k), for 0 <= k <= n.
 * https://oeis.org/A269944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 105,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269944 – Triangle read by rows
 *  T(0,0) = 1,
 *  T(n,0) = 0  for n > 0,
 *  T(n,k) = T(n-1,k-1) + (n-1)^2 * T(n-1,k)   (0 ≤ k ≤ n)
 *
 *  a(n) is the n‑th entry of the flat row‑major traversal of the
 *  triangle T; i.e. a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

static seq_t **triangle = NULL;  /* triangle[row] points to array of T(row,0..row) */
static size_t rows_allocated = 0;/* number of rows already built */

/* Build all rows up to and including max_row, allocating memory as needed. */
static void ensure_rows(idx_t max_row)
{
    if ((idx_t)rows_allocated > max_row)        /* already present */
        return;

    size_t new_rows = (size_t)(max_row + 1);
    triangle = (seq_t **)realloc(triangle, new_rows * sizeof *triangle);
    for (size_t r = rows_allocated; r < new_rows; ++r) {
        triangle[r] = (seq_t *)malloc((r + 1) * sizeof *triangle[r]);
        for (size_t k = 0; k <= r; ++k) {
            if (r == 0) {                     /* only element in row 0 */
                triangle[r][k] = (seq_t)1;
            } else if (k == 0) {              /* T(n,0)=0 for n>0 */
                triangle[r][k] = (seq_t)0;
            } else {
                seq_t prev_prev = triangle[r-1][k-1];
                seq_t prev = (k <= r-1) ? triangle[r-1][k] : (seq_t)0;
                seq_t factor = (seq_t)(r-1) * (r-1);   /* (n‑1)^2 */
                triangle[r][k] = prev_prev + factor * prev;
            }
        }
    }
    rows_allocated = new_rows;
}

/* Convert a flat index n into the corresponding (row, column) in the triangle.
 * Invariant: row*(row+1)/2 ≤ n < (row+1)*(row+2)/2 .
 */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    while (n >= (r + 1) * (r + 2) / 2)
        ++r;
    *row = r;
    *col = n - r * (r + 1) / 2;
}

/* External interface requested by the benchmark harness. */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_row_col(n, &r, &c);
    ensure_rows(r);
    return triangle[r][c];
}
