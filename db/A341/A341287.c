/*
 * A341287  Triangle read by rows: T(n,k) = Sum_{i=0..n-1} binomial(n-1,
 *          i)*T(n-1-i,k-1) - Sum_{i=1..n-1} binomial(n-1,i)*T(n-1-i,k) for 1
 *          <= k <= n+1 with T(0,1) = 1 (and T(n,k) = 0 otherwise).
 * https://oeis.org/A341287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the (1‑based) flattening of the triangle T(n,k) defined
 *            by T(0,1)=1 and, for n≥1, 1≤k≤n+1, T(n,k)=∑_{i=0}^{n-1}
 *            binomial(n-1,i)*T(n-1-i,k-1) –∑_{i=1}^{n-1}
 *            binomial(n-1,i)*T(n-1-i,k)
 * Range:     i8 n <= 28, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the nth term (n≥0). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"      /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_alloc_new_array.h"  /* seq_alloc_new_array(size_t n) */

static seq_t **triangle = NULL;        /* triangular array: triangle[row][k] */
static size_t max_row = (size_t)-1;    /* highest row computed */

/* ensure triangle[0] exists */
static void init_triangle(void) {
    if (triangle == NULL) {
        triangle = (seq_t**)malloc(sizeof(seq_t*));
        if (!triangle) { abort(); }
        triangle[0] = seq_alloc_new_array(2);   /* indices 0..1; use 1..1 */
        triangle[0][1] = (seq_t)1;
        max_row = 0;
    }
}

/* compute a single row n (n≥1) and store it */
static void compute_row(idx_t n) {
    /* allocate space for row n: indices 0..n+1, use 1..n+1 */
    seq_t *row = seq_alloc_new_array(n + 2);
    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t sum1 = 0;
        seq_t sum2 = 0;
        for (idx_t i = 0; i < n; ++i) {          /* i = 0 .. n-1 */
            seq_t bin = binomial(n - 1, i);
            idx_t r1 = n - 1 - i;
            idx_t c1 = k - 1;
            seq_t val1 = 0;
            if (c1 >= 1 && c1 <= r1 + 1) {
                val1 = triangle[r1][c1];
            }
            sum1 += bin * val1;

            if (i >= 1) {                       /* second sum starts at i=1 */
                idx_t r2 = n - 1 - i;
                idx_t c2 = k;
                seq_t val2 = 0;
                if (c2 >= 1 && c2 <= r2 + 1) {
                    val2 = triangle[r2][c2];
                }
                sum2 += bin * val2;
            }
        }
        row[k] = sum1 - sum2;
    }

    /* grow triangle array if necessary */
    if (triangle == NULL) { abort(); }
    if (n > max_row) {
        /* reallocate pointer array to accommodate new row */
        triangle = (seq_t**)realloc(triangle, (n + 1) * sizeof(seq_t*));
        if (!triangle) { abort(); }
    }
    triangle[n] = row;
    if (n > max_row) max_row = n;
}

/* ensure rows up to needed_row have been computed */
static void ensure_rows(idx_t needed_row) {
    init_triangle();
    while (max_row < needed_row) {
        compute_row((idx_t)(max_row + 1));
    }
}

/* convert global index n to (row,k) */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col) {
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    *row = r;
    *col = n - cum + 1;
}

/* public function required by the specification */
seq_t a(idx_t n) {
    idx_t row, col;
    index_to_rowcol(n, &row, &col);
    ensure_rows(row);
    return triangle[row][col];
}
