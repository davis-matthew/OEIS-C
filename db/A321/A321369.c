/*
 * A321369  Coefficients of successive polynomials formed by iterating f(x) =
 *          -1 + 2x^2. Irregular triangle read by rows.
 * https://oeis.org/A321369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   a(n) is the nth term of the flattened triangle formed by the
 *            coefficients * of the polynomials obtained by iterating f(x) =
 *            -1 + 2x^2. * The triangle rows are defined by: * T(0,0) = 1, *
 *            row 1: T(1,0) = -1, T(1,1) = 2, * and for n >= 2: * T(n,0) = -1
 *            + 2 * (T(n-1,0))^2, * T(n,k) = 2 * Σ_{i=0..k}...
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 26, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for memoization */
static seq_t **rows      = NULL;  /* pointers to each row array */
static idx_t *row_len    = NULL;  /* number of elements in each row */
static idx_t *prefix     = NULL;  /* cumulative number of terms up to each row */
static idx_t  row_cnt    = 0;     /* number of stored rows */

/* Compute and store row r (0‑based).  Caller must ensure that rows < r-1 are already computed. */
static void add_row(idx_t r)
{
    seq_t *new_row;
    idx_t new_len;

    if (r == 0) { /* Row 0: [1] */
        new_len = 1;
        new_row = (seq_t*)malloc(sizeof(seq_t));
        new_row[0] = (seq_t)1;
    }
    else if (r == 1) { /* Row 1: [-1, 2] */
        new_len = 2;
        new_row = (seq_t*)malloc(sizeof(seq_t) * new_len);
        new_row[0] = (seq_t)-1;
        new_row[1] = (seq_t)2;
    }
    else {
        /* Use recurrence to build row r from row r-1. */
        seq_t *prev = rows[r - 1];
        idx_t prev_len = row_len[r - 1];
        new_len = 2 * prev_len - 1;              /* length for this row */
        new_row = (seq_t*)calloc((size_t)new_len, sizeof(seq_t));

        for (idx_t k = 0; k < new_len; ++k) {
            idx_t i_start = (k >= prev_len) ? k - (prev_len - 1) : (idx_t)0;
            idx_t i_end   = (k < prev_len)       ? k : prev_len - 1;
            seq_t sum = (seq_t)0;
            for (idx_t i = i_start; i <= i_end; ++i)
                sum += prev[i] * prev[k - i];
            new_row[k] = sum * (seq_t)2;
        }
        new_row[0] -= (seq_t)1;   /* subtract the constant term */
    }

    /* Grow storage arrays. */
    rows     = (seq_t**)realloc(rows,     sizeof(seq_t*) * (r + 1));
    row_len  = (idx_t*)realloc(row_len,  sizeof(idx_t)  * (r + 1));
    prefix   = (idx_t*)realloc(prefix,   sizeof(idx_t)  * (r + 1));

    rows[r]     = new_row;
    row_len[r]  = new_len;
    prefix[r]   = (r == 0) ? new_len : prefix[r - 1] + new_len;
    row_cnt = r + 1;
}

/* Public function to return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure enough rows are precomputed to cover index n. */
    while (row_cnt == 0 || prefix[row_cnt - 1] <= n)
        add_row(row_cnt);

    /* Locate the row containing index n. */
    idx_t r = 0;
    while (r < row_cnt && prefix[r] <= n)
        ++r;

    /* Compute offset within that row. */
    idx_t prev_prefix = (r == 0) ? (idx_t)0 : prefix[r - 1];
    idx_t offset = n - prev_prefix;

    return rows[r][offset];
}
