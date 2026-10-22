/*
 * A316939  Triangle read by rows formed using Pascal's rule except that n-th
 *          row begins and ends with Fibonacci(n+2).
 * https://oeis.org/A316939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle rows are defined recursively:
   * each row n has length n+1,
   * the first and last entry of row n are Fibonacci(n+2),
   * each interior entry is the sum of the two entries above it
     in the previous row.
   a(n) is the n‑th element of this triangle when it is read row‑by‑row. */

#include <stddef.h>
#include <stdlib.h>

/* Basic Fibonacci function (fib(0)=0, fib(1)=1). */
static seq_t fib(idx_t n)
{
    seq_t a = 0;
    seq_t b = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t tmp = a;
        a = b;
        b = tmp + b;
    }
    return a;
}

/* Static storage for the triangle rows. */
static seq_t **rows      = NULL;              /* array of pointers to rows */
static size_t rows_cap   = 0;                 /* allocated capacity for rows[] */
static idx_t  max_row_computed = (idx_t)-1;    /* highest row fully built */

/* Ensure that rows[] can hold at least (n+1) pointers. */
static void ensure_rows_capacity(idx_t n)
{
    if (rows_cap <= (size_t)n + 1) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) exit(1);            /* allocation failed */
        rows = tmp;
        rows_cap = new_cap;
    }
}

/* Build all rows up to index 'r' (inclusive). */
static void compute_rows_up_to(idx_t r)
{
    /* Build row 0 first, if not already done. */
    if (max_row_computed == (idx_t)-1) {
        ensure_rows_capacity(0);
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!rows[0]) exit(1);
        rows[0][0] = fib((idx_t)2);      /* Fib(2) = 1 */
        max_row_computed = 0;
    }

    /* Append rows until we reach 'r'. */
    for (idx_t k = max_row_computed + 1; k <= r; ++k) {
        size_t len = (size_t)k + 1;
        ensure_rows_capacity(k);
        rows[k] = (seq_t*)malloc(len * sizeof(seq_t));
        if (!rows[k]) exit(1);

        seq_t edge = fib(k + 2);          /* Fibonacci(k+2) */
        rows[k][0] = edge;
        rows[k][k] = edge;
        for (size_t j = 1; j + 1 < len; ++j) {
            rows[k][j] = rows[k-1][j-1] + rows[k-1][j];
        }
    }
    max_row_computed = r;
}

/* Return the nth term of the sequence, for n ≥ 0. */
seq_t a(idx_t n)
{
    /* Find the row 'r' that contains index n:
       r is the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    idx_t tri = 0;                       /* T(r) = r*(r+1)/2 */
    while (tri + (r + 1) <= n) {
        tri += (r + 1);
        r++;
    }

    idx_t start = tri;                   /* start index of row r */
    idx_t pos   = n - start;             /* column in row r */

    compute_rows_up_to(r);                /* ensure row r exists */
    return rows[r][pos];
}
