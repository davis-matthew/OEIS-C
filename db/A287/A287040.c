/*
 * A287040  Triangle T(n,k) read by rows: coefficients of polynomials P_n(t)
 *          defined in Formula section.
 * https://oeis.org/A287040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th entry of the triangle of coefficients of the
 *            polynomials P_n(t) defined by the recurrence y = t + x*y^2 +
 *            x*deriv(y,t) + x*y. The triangle is read by rows, each row n
 *            containing n+2 coefficients
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 106, u64 n <= 301,
 *            u128 n <= 905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t **rows = NULL;
static size_t rows_count = 0;
static size_t rows_cap   = 0;

/* Ensure that rows[0..target] are computed and stored. */
static void ensure_rows_up_to(size_t target)
{
    if (target < rows_count) return;

    /* Resize outer array if needed. */
    size_t new_cap = rows_cap;
    if (new_cap < 2) new_cap = 2;
    while (new_cap <= target) new_cap <<= 1;
    if (new_cap != rows_cap) {
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute missing rows. */
    while (rows_count <= target) {
        size_t n      = rows_count;
        size_t len    = n + 2;          /* number of coefficients in this row */
        seq_t *row    = (seq_t*)calloc(len, sizeof(seq_t));
        if (!row) abort();

        if (n == 0) {
            row[1] = (seq_t)1;          /* P0 = t  →  coefficient t^1 = 1 */
        } else {
            seq_t *prev = rows[n-1];

            /* derivative part and previous polynomial part. */
            for (size_t k = 0; k < n; ++k) {
                row[k] += (seq_t)(k+1) * prev[k+1];   /* derivative part */
                row[k] += prev[k];                   /* previous part   */
            }
            row[n] += prev[n];                       /* only prev contributes at degree n */

            /* convolution part  (P_i * P_{n-1-i}) */
            for (size_t i = 0; i <= n-1; ++i) {
                size_t j = n-1-i;
                seq_t *ri = rows[i];
                seq_t *rj = rows[j];
                for (size_t a = 0; a <= i+1; ++a) {
                    seq_t va = ri[a];
                    if (!va) continue;
                    for (size_t b = 0; b <= j+1; ++b) {
                        seq_t vb = rj[b];
                        if (!vb) continue;
                        row[a+b] += va * vb;
                    }
                }
            }
        }

        rows[rows_count++] = row;
    }
}

/* Return the n-th term of the sequence A287040. */
seq_t a(idx_t n)
{
    /* Find the row r such that S(r-1) <= n < S(r).  S(r) = (r+1)(r+4)/2. */
    idx_t r = 0;
    while (1) {
        idx_t count = ((r+1) * (r+4)) / 2;
        if (count > n) break;
        ++r;
    }
    idx_t start  = (r * (r+3)) / 2;          /* first index of this row */
    idx_t offset = n - start;                /* column within the row */
    size_t row_idx = (size_t)r;
    size_t col_idx = (size_t)offset;

    ensure_rows_up_to(row_idx);
    return rows[row_idx][col_idx];
}
