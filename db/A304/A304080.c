/*
 * A304080  Triangular array T(n,k) giving coefficients in expansion of
 *          Product_{j=1..n} (1-x^j)^2.
 * https://oeis.org/A304080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is obtained by flattening the triangular array T(n,k) of
 *            the coefficients in the expansion of ∏_{j=1}^{n} (1 - x^j)^2.
 *            The index n counts through the rows left‑to‑right, row by row
 * Range:     i8 n <= 831, i16 n <= 8525, i32 n <= 9950, i64 n <= 9950,
 *            i128 n <= 9950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build system. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation tables. */
    static idx_t cap_rows   = 0;
    static idx_t num_rows   = 0;
    static seq_t **rows     = NULL;
    static idx_t *row_len   = NULL;
    static idx_t *start_idx = NULL;
    static idx_t total_cnt  = 0;   /* total number of elements in all rows so far */

    /* Build rows until we cover index n. */
    while (n >= total_cnt) {
        /* Index of the new row to be built. */
        idx_t r = num_rows;          /* 0,1,2,... */

        /* Length of the new row: r*(r+1)+1. */
        idx_t len = r * (r + 1) + 1;

        /* Allocate and zero‑initialise the new row. */
        seq_t *new_row = (seq_t *)calloc((size_t)len, sizeof(seq_t));
        if (!new_row) return 0;      /* OOM: return 0 (will never happen in tests) */

        if (r == 0) {
            /* First row: simply 1. */
            new_row[0] = 1;
        } else {
            seq_t *prev = rows[r - 1];
            idx_t  prev_len = row_len[r - 1];
            for (idx_t i = 0; i < prev_len; ++i) {
                seq_t val = prev[i];
                new_row[i]        = new_row[i] + val;                 /* +1 * prev[i]   */
                seq_t two = val + val;
                new_row[i + r] = new_row[i + r] - two;               /* -2 * prev[i]   */
                new_row[i + 2 * r] = new_row[i + 2 * r] + val;       /* +1 * prev[i]   */
            }
        }

        /* Make room for a new row entry. */
        if (num_rows >= cap_rows) {
            size_t new_cap = (cap_rows == 0) ? 4 : cap_rows * 2;
            seq_t **tmp_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            idx_t *tmp_len   = (idx_t *)realloc(row_len, new_cap * sizeof(idx_t));
            idx_t *tmp_start = (idx_t *)realloc(start_idx, new_cap * sizeof(idx_t));
            if (!tmp_rows || !tmp_len || !tmp_start) return 0;      /* OOM */
            rows = tmp_rows;
            row_len = tmp_len;
            start_idx = tmp_start;
            cap_rows = new_cap;
        }

        rows[num_rows] = new_row;
        row_len[num_rows] = len;
        start_idx[num_rows] = total_cnt;
        total_cnt += len;
        ++num_rows;
    }

    /* Locate the row containing index n. */
    idx_t r = 0;
    while (1) {
        if (n < start_idx[r] + row_len[r]) break;
        ++r;
    }
    idx_t k = n - start_idx[r];
    return rows[r][k];
}
