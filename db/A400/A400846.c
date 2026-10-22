/*
 * A400846  Irregular triangular array read by rows. T(n,k) is the number of
 *          ordered set partitions of [n] whose associated composition c is
 *          such that f(c)=k, 0<=k<=binomial(n,2), n>=0 (where associated
 *          composition and f(c) are defined below in the comments section).
 * https://oeis.org/A400846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, new
 * Formula:   a(n) = T(row(n),col(n)) where * T(0,0) = 1 * for n>=1: T(n,k) =
 *            Σ_{a=1..n} C(n,a) * T(n-a, k - a*(n-a)) * (C(n,a) is
 *            binomial(n,a).) * The triangle is read by rows so that * a(idx)
 *            = T(n,k) with idx = row_start[n] + k
 * Range:     u8 n <= 36, u16 n <= 87, u32 n <= 293, u64 n <= 1335,
 *            u128 n <= 5469, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b) */

static size_t dp_rows = 0;          /* number of rows currently allocated */
static seq_t **dp_rows_ptr = NULL;  /* array of pointers to each row */
static idx_t *row_kmax = NULL;      /* maximum k value for each row (n*(n-1)/2) */
static idx_t *row_start = NULL;     /* starting flat index of each row */

/* Ensure the DP arrays are allocated up to (and including) row 'new_rows-1'.
   This may recompute all rows from scratch (cheap because new_rows ≤ ~40). */
static void grow_rows_to(idx_t new_rows)
{
    size_t old_rows = dp_rows;
    if (new_rows <= dp_rows) return;          /* already enough */

    /* Reallocate metadata arrays for the new rows. */
    dp_rows_ptr = (seq_t **)realloc(dp_rows_ptr,
                                    (size_t)new_rows * sizeof(seq_t *));
    row_kmax    = (idx_t *)realloc(row_kmax,
                                   (size_t)new_rows * sizeof(idx_t));
    row_start   = (idx_t *)realloc(row_start,
                                   (size_t)new_rows * sizeof(idx_t));

    /* Allocate new rows and compute their values. */
    for (idx_t i = (idx_t)old_rows; i < new_rows; ++i) {
        idx_t kmax = (idx_t)((i * (i - 1)) / 2); /* binomial(i,2) */
        row_kmax[i] = kmax;

        /* Allocate storage for this row and zero it. */
        dp_rows_ptr[i] = (seq_t *)calloc((size_t)(kmax + 1), sizeof(seq_t));

        /* Record the starting index of this row in the flat sequence. */
        if (i == 0) {
            row_start[i] = 0;
        } else {
            row_start[i] = row_start[i - 1] + (idx_t)(row_kmax[i - 1] + 1);
        }

        /* Fill the row via the recurrence if n > 0. */
        if (i > 0) {
            for (idx_t a = 1; a <= i; ++a) {
                idx_t rem   = i - a;
                seq_t comb  = binomial((idx_t)i, (idx_t)a);    /* C(i,a) */
                idx_t kmax_rem = row_kmax[rem];

                /* Loop over all k' values of the smaller row. */
                for (idx_t kprime = 0; kprime <= kmax_rem; ++kprime) {
                    idx_t k = kprime + a * rem;
                    seq_t add = comb * dp_rows_ptr[rem][kprime];
                    dp_rows_ptr[i][k] += add;
                }
            }
        } else {
            /* Base case: T(0,0) = 1. */
            dp_rows_ptr[0][0] = (seq_t)1;
        }
    }

    dp_rows = new_rows;
}

/* Find the row 'r' that contains the flat index 'n'.
   Rows are indexed from 0. */
static idx_t find_row(idx_t n)
{
    /* Extend the tables until the requested index lies inside the last row. */
    while (dp_rows == 0 ||
           n >= row_start[dp_rows - 1] + (row_kmax[dp_rows - 1] + 1)) {
        size_t grow_to = dp_rows + 1;
        grow_rows_to((idx_t)grow_to);
    }

    /* Linear search: rows are very few (~40 for n≤10000). */
    for (idx_t r = 0; r < dp_rows; ++r) {
        idx_t r_end = row_start[r] + (row_kmax[r] + 1);
        if (n < r_end) {
            return r;
        }
    }
    /* Should never happen */
    return dp_rows - 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Ensure at least row 0 is computed. */
    if (dp_rows == 0) grow_rows_to(1);

    idx_t row = find_row(n);
    idx_t col = n - row_start[row];
    return dp_rows_ptr[row][col];
}
