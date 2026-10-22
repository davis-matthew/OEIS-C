/*
 * A355659  Martingale win/loss triangle, read by rows: T(n,k) = total number
 *          of dollars won (or lost) using the martingale method on all
 *          possible n trials that have exactly k losses and n-k wins, for 0
 *          <= k <= n.
 * https://oeis.org/A355659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the entry T(n, k) from the Martingale win/loss triangle
 *            read by rows. The triangle satisfies: T(0,0) = 0 T(n,0) = n
 *            T(n,n) = 1 - 2^n T(n,k) = T(n-1,k) + T(n-1,k-1) + C(n-1,k)
 *            (0<k<n) where C(a,b) is the binomial coefficient
 * Range:     i8 n <= 37, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t n, idx_t k); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

static seq_t **row_table = NULL;   /* row_table[i] points to the array of T(i, *) */
static idx_t max_cached = (idx_t)-1;  /* highest row that has been cached */

/* Build all rows 0 .. rmax if not already present. */
static void build_rows(idx_t rmax)
{
    if (row_table == NULL) {
        /* First allocation of the table of row pointers. */
        row_table = (seq_t **)malloc((size_t)(rmax + 1) * sizeof(seq_t *));
        for (idx_t i = 0; i <= rmax; ++i) row_table[i] = NULL;
        max_cached = (idx_t)-1;
    } else if (max_cached < rmax) {
        row_table = (seq_t **)realloc((void *)row_table,
                                      (size_t)(rmax + 1) * sizeof(seq_t *));
        for (idx_t i = max_cached + 1; i <= rmax; ++i) row_table[i] = NULL;
    }

    for (idx_t i = max_cached + 1; i <= rmax; ++i) {
        row_table[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));

        /* Berie boundary values. */
        row_table[i][0] = (seq_t)i;                   /* T(i,0) = i */
        seq_t pow_val = pow2(i);                       /* 2^i */
        row_table[i][i] = 1 - pow_val;                /* T(i,i) = 1-2^i */

        /* Interior values via the recurrence. */
        for (idx_t k = 1; k < i; ++k) {
            row_table[i][k] =
                row_table[i-1][k] + row_table[i-1][k-1] + binom(i-1, k);
        }
    }
    max_cached = rmax;
}

/* Convert a global index n into (row, col) in the triangle. */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    idx_t pref = 0;
    idx_t r    = 0;
    while (pref + r + 1 <= n) {
        pref += r + 1;
        ++r;
    }
    *row = r;
    *col = n - pref;
}

/* Return the a(n) term. */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rowcol(n, &r, &c);
    build_rows(r);            /* guarantee row r is computed */
    return row_table[r][c];
}
