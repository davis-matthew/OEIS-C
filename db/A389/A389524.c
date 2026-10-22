/*
 * A389524  Triangle read by rows. T(1, 1) = 1, T(n, k) = [n >= k](1 +
 *          Sum_{i=1..k-1} T(n - i, k - 1) - Sum_{i=1..n-1} T(n - i, k)).
 * https://oeis.org/A389524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Triangle T(n,k) defined by recurrence * * T(1,1) = 1 * T(n,k) =
 *            [n >= k] * ( 1 + Σ_{i=1}^{k-1} T(n-i, k-1) * - Σ_{i=1}^{n-1}
 *            T(n-i, k) ) * Where [n >= k] is 1 if n>=k, else 0. * * The
 *            function a(n) returns the flattened triangle read by rows. *
 *            That is, a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)...
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */
extern seq_t a(idx_t n);

/* Local definitions ------------------------------------------------------- */

#define MAX_ROWS 200    /* sufficient for n <= 10000 (row ≈ 141) */

static seq_t *rows[MAX_ROWS + 1];   /* rows[1..max_rows] hold the triangle */
static idx_t max_rows = 0;          /* number of rows currently allocated */

/* Helper: ensure that rows are computed up to 'target_row' */
static void ensure_rows_up_to(idx_t target_row)
{
    while (max_rows < target_row) {
        ++max_rows;
        /* Allocate space for the new row (1-based indexing). */
        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * (max_rows + 1));
        if (!row) {
            /* Allocation failed – abort program. */
            abort();
        }
        row[0] = 0;   /* unused */
        for (idx_t k = 1; k <= max_rows; ++k) {
            if (max_rows == 1 && k == 1) {
                /* Base case */
                row[1] = 1;
            } else {
                seq_t sum_prev_col = 0;
                /* Σ_{i=1}^{k-1} T(n-i, k-1)  (window on column k-1) */
                for (idx_t i = 1; i < k; ++i) {
                    idx_t r = max_rows - i;
                    if (r >= k - 1) {
                        sum_prev_col += rows[r][k - 1];
                    }
                }
                seq_t sum_col = 0;
                /* Σ_{i=1}^{n-1} T(n-i, k)    (sum over column k up to row n-1) */
                for (idx_t i = 1; i < max_rows; ++i) {
                    idx_t r = max_rows - i;
                    if (r >= k) {
                        sum_col += rows[r][k];
                    }
                }
                row[k] = (seq_t)1 + sum_prev_col - sum_col;
            }
        }
        rows[max_rows] = row;
    }
}

/* Main function ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined by OEIS, but safe to return 0 */
    }

    /* Determine the row index 'r' such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((seq_t)r * ((seq_t)r + 1) / 2 < n) {
        ++r;
    }

    /* Build rows up to 'r' if necessary. */
    ensure_rows_up_to(r);

    /* Column index within row r */
    seq_t previous_rows_total = (seq_t)(r - 1) * (seq_t)r / 2;
    idx_t k = n - (idx_t)previous_rows_total;  /* 1‑based */

    return rows[r][k];
}
