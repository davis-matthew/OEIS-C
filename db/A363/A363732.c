/*
 * A363732  Triangle read by rows. The triangle algorithm applied to
 *          (-1)^n/n!.
 * https://oeis.org/A363732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 36, i32 n <= 105, i64 n <= 300,
 *            i128 n <= 902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows of the triangle algorithm applied to a(m)=(-1)^m/m! */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that the index n lies in */
    idx_t r = 0, cumulative = 0;
    for (idx_t row = 0;; ++row) {
        cumulative += row + 1;          /* number of terms in row 'row' */
        if (cumulative > n) {
            r = row;
            break;
        }
    }

    /* position k within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate 2‑D arrays of size (r+1) x (r+1) */
    size_t dim = (size_t)(r + 1);
    seq_t **prev = (seq_t**)malloc(dim * sizeof(seq_t*));
    seq_t **cur  = (seq_t**)malloc(dim * sizeof(seq_t*));
    for (idx_t m = 0; m <= r; ++m) {
        prev[m] = (seq_t*)calloc(dim, sizeof(seq_t));
        cur[m]  = (seq_t*)calloc(dim, sizeof(seq_t));
    }

    /* Base row: p(0,m,x) = (-1)^m (constant polynomial) */
    for (idx_t m = 0; m <= r; ++m)
        prev[m][0] = (m & 1U) ? -1 : 1;

    /* Build subsequent rows */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t m = 0; m <= r; ++m) {
            for (idx_t d = 0; d <= row; ++d) {
                seq_t val = 0;
                /* term from p(row-1, m+1, x) */
                if (m < r)
                    val += prev[m + 1][d];
                /* - (m+1) * p(row-1, m, x) */
                if (d <= row - 1)
                    val -= (seq_t)(m + 1) * prev[m][d];
                /* + x * p(row-1, m, x)  -> shift coefficients */
                if (d > 0)
                    val += prev[m][d - 1];
                cur[m][d] = val;
            }
        }
        /* Prepare for next iteration: swap prev and cur */
        for (idx_t m = 0; m <= r; ++m) {
            seq_t *tmp = prev[m];
            prev[m] = cur[m];
            cur[m] = tmp;
        }
    }

    seq_t result = prev[0][k];

    /* Free allocated memory */
    for (idx_t m = 0; m <= r; ++m) {
        free(prev[m]);
        free(cur[m]);
    }
    free(prev);
    free(cur);

    return result;
}
