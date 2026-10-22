/*
 * A394437  Triangle read by rows: The Bell-transform of the squares.
 * https://oeis.org/A394437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = B_{row,col} where a_j = j² (B_{n,k} are partial Bell
 *            polynomials of the square sequence). The triangle is read by
 *            rows: row 0: 1 row r: B_{r,0} … B_{r,r}
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

static seq_t **tri = NULL;            /* triangular cache */
static idx_t max_row_alloc = 0;       /* highest row already cached */

/* Ensure that rows 0 … r are computed and cached */
static void ensure_rows_up_to(idx_t r)
{
    if (tri == NULL) {
        tri = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
        tri[0] = (seq_t*)malloc(sizeof(seq_t));
        tri[0][0] = 1;                 /* B_{0,0} */
        max_row_alloc = 0;
    }
    if (r <= max_row_alloc)
        return;

    /* grow pointer array */
    seq_t **new_tri = (seq_t**)realloc(tri, (r + 1) * sizeof(seq_t*));
    tri = new_tri;

    /* allocate new rows */
    for (idx_t n = max_row_alloc + 1; n <= r; ++n) {
        tri[n] = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= n; ++k)
            tri[n][k] = 0;
    }

    /* compute new rows */
    for (idx_t n = max_row_alloc + 1; n <= r; ++n) {
        /* B_{n,0} stays 0 for n>0 */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = 0;
            idx_t maxj = n - k + 1;
            for (idx_t j = 1; j <= maxj; ++j) {
                seq_t bin = binomial(n - 1, j - 1);   /* C(n-1, j-1) */
                seq_t a_j = (seq_t)j * j;             /* j²          */
                seq_t prev = tri[n - j][k - 1];       /* B_{n-j,k-1} */
                sum += bin * a_j * prev;
            }
            tri[n][k] = sum;
        }
    }

    max_row_alloc = r;
}

/* Convert linear index n to (row, col) of the triangle */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    /* find largest r with r*(r+1)/2 <= n */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    *row = r;
    *col = n - r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_rowcol(n, &row, &col);
    ensure_rows_up_to(row);
    return tri[row][col];
}
