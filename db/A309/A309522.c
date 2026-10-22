/*
 * A309522  Generalized Blasius numbers, square array read by ascending
 *          antidiagonals, A(n, k) for n, k >= 0.
 * https://oeis.org/A309522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A309522(n) – the square array read by ascending *
 *            antidiagonals. A(n,k) = Sum_{v=0..k-1} binomial(n*k-1, n*v) * *
 *            A(n,v) * A(n,k-1-v) for k>0, A(n,0)=1. * The array is accessed
 *            by converting the linear index n to * coordinates (row, col)
 *            with the rule: * d(d+1)/2 ≤ n < (d+1)(d+2)/2 * offse...
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

static seq_t **rows      = NULL;   /* rows[row][k] = A(row,k)   */
static size_t *row_cap   = NULL;   /* capacity of each row */
static size_t *row_len   = NULL;   /* number of computed entries in each row */
static size_t rows_cap   = 0;      /* total rows stored */

/* Ensure the bookkeeping arrays can hold rows [0..need] */
static void ensure_rows_capacity(idx_t need)
{
    if ((size_t)need < rows_cap) return;
    size_t old = rows_cap;
    rows_cap   = (size_t)need + 1;
    rows      = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
    row_cap   = (size_t *)realloc(row_cap, rows_cap * sizeof(size_t));
    row_len   = (size_t *)realloc(row_len, rows_cap * sizeof(size_t));
    for (size_t i = old; i < rows_cap; ++i) {
        rows[i]    = NULL;
        row_cap[i] = 0;
        row_len[i] = 0;
    }
}

/* Compute A(row,k) for all k in [row_len[row] … col] (inclusive) */
static void ensure_row(idx_t row, idx_t col)
{
    ensure_rows_capacity(row);

    if (row_cap[row] < (size_t)(col + 1)) {
        /* grow memory block to new capacity */
        size_t new_cap = (size_t)(col + 1);
        size_t old_cap = row_cap[row];
        seq_t *tmp = (seq_t *)realloc(rows[row], new_cap * sizeof(seq_t));
        /* Initialise newly allocated slots to 0 */
        for (size_t i = old_cap; i < new_cap; ++i) tmp[i] = 0;
        rows[row] = tmp;
        row_cap[row] = new_cap;
    }

    /* Compute missing entries sequentially */
    for (size_t k = row_len[row]; k <= (size_t)col; ++k) {
        if (row == 0) {
            rows[row][k] = catalan((idx_t)k);
        } else if (k == 0) {
            rows[row][k] = 1;
        } else {
            seq_t sum = 0;
            for (size_t v = 0; v < (size_t)k; ++v) {
                seq_t bin = binomial(row * (idx_t)k - 1, row * (idx_t)v);
                seq_t term = bin * rows[row][v] * rows[row][(size_t)k - 1 - v];
                sum += term;
            }
            rows[row][k] = sum;
        }
    }
    row_len[row] = (size_t)col + 1;
}

/* Convert linear index n to (row, col) in the antidiagonal enumeration */
static void index_to_coords(idx_t n, idx_t *row, idx_t *col)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    idx_t prev = d * (d + 1) / 2;
    idx_t offset = n - prev;
    *row = d - offset;
    *col = offset;
}

seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_coords(n, &row, &col);
    ensure_row(row, col);
    return rows[row][col];
}
