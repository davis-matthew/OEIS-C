/*
 * A354666  Triangle read by rows, T(n,k) = T(n-1,k) + T(n-2,k-1) +
 *          2*T(n-2,k-2) - T(n-3,k-1) - T(n-3,k-2) + T(n-4,k-1) + T(n-4,k-2) -
 *          T(n-4,k-3) - T(n-4,k-4) + delta(n,0)*delta(k,0) -
 *          delta(n,2)*(delta(k,1) + delta(k,2)), T(n<k,k) = T(n,k<0) = 0.
 * https://oeis.org/A354666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for triangle T(n,k):
 *   T(n,k) = T(n-1,k) + T(n-2,k-1) + 2*T(n-2,k-2)
 *            - T(n-3,k-1) - T(n-3,k-2)
 *            + T(n-4,k-1) + T(n-4,k-2)
 *            - T(n-4,k-3) - T(n-4,k-4)
 *            + delta(n,0)*delta(k,0)
 *            - delta(n,2)*(delta(k,1)+delta(k,2))
 *   with T(n,k)=0 for n<k or k<0, and T(n,0)=1.
 * Sequence a(n) is the triangle read by rows (flattened).
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t **triangle_rows = NULL;      /* pointers to rows of the triangle */
static size_t   rows_computed = 0;        /* number of rows already computed */

/* Ensure that rows 0 … need_row are computed and stored. */
static void ensure_rows(idx_t need_row)
{
    /* If we already have these rows, nothing to do. */
    if ((size_t)(need_row + 1) <= rows_computed)
        return;

    /* Make space for the additional rows (if needed). */
    size_t new_cap = need_row + 1;
    if (!triangle_rows) {
        triangle_rows = (seq_t **)malloc(new_cap * sizeof(seq_t *));
    } else if (new_cap > rows_computed) {
        triangle_rows = (seq_t **)realloc(triangle_rows,
                                          new_cap * sizeof(seq_t *));
    }

    /* Compute missing rows sequentially. */
    for (size_t r = rows_computed; r <= (size_t)need_row; ++r) {
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        for (size_t c = 0; c <= r; ++c) {
            seq_t val = 0;

            /* T(r-1,c) */
            if (r >= 1 && c <= r - 1)
                val += triangle_rows[r - 1][c];

            /* T(r-2,c-1) */
            if (r >= 2 && c >= 1 && c - 1 <= r - 2)
                val += triangle_rows[r - 2][c - 1];

            /* 2 * T(r-2,c-2) */
            if (r >= 2 && c >= 2 && c - 2 <= r - 2)
                val += (seq_t)2 * triangle_rows[r - 2][c - 2];

            /* - T(r-3,c-1) */
            if (r >= 3 && c >= 1 && c - 1 <= r - 3)
                val -= triangle_rows[r - 3][c - 1];

            /* - T(r-3,c-2) */
            if (r >= 3 && c >= 2 && c - 2 <= r - 3)
                val -= triangle_rows[r - 3][c - 2];

            /* + T(r-4,c-1) */
            if (r >= 4 && c >= 1 && c - 1 <= r - 4)
                val += triangle_rows[r - 4][c - 1];

            /* + T(r-4,c-2) */
            if (r >= 4 && c >= 2 && c - 2 <= r - 4)
                val += triangle_rows[r - 4][c - 2];

            /* - T(r-4,c-3) */
            if (r >= 4 && c >= 3 && c - 3 <= r - 4)
                val -= triangle_rows[r - 4][c - 3];

            /* - T(r-4,c-4) */
            if (r >= 4 && c >= 4 && c - 4 <= r - 4)
                val -= triangle_rows[r - 4][c - 4];

            /* + delta(r,0)*delta(c,0) */
            if (r == 0 && c == 0)
                val += (seq_t)1;

            /* - delta(r,2)*(delta(c,1)+delta(c,2)) */
            if (r == 2 && (c == 1 || c == 2))
                val -= (seq_t)1;

            row[c] = val;
        }
        triangle_rows[r] = row;
    }

    rows_computed = need_row + 1;
}

/* Main sequence function: a(n) is the nth term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Determine the row r that contains position n. */
    size_t r = 0;
    size_t tri = 1;  /* (r+1)(r+2)/2 for r = 0 */
    while (tri <= (size_t)n) {
        ++r;
        tri = (size_t)(r + 1) * (r + 2) / 2;
    }

    /* Column index within that row. */
    size_t prev_tri = (size_t)r * (r + 1) / 2;
    size_t c = (size_t)n - prev_tri;

    /* Ensure the required rows are computed. */
    ensure_rows((idx_t)r);

    /* Return the requested value. */
    return triangle_rows[r][c];
}
