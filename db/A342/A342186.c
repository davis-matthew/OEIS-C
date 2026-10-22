/*
 * A342186  Triangle read by rows, matrix inverse of A139382.
 * https://oeis.org/A342186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) row by row,
 *            where T satisfies: T(1,1) = 1, T(n,0) = 0, T(n,k) = T(n-1,k-1) -
 *            (2^(n-1)-1) * T(n-1,k) for n,k >= 1. a(1) = T(1,1), a(2) =
 *            T(2,1), a(3) = T(2,2), a(4) = T(3,1), …
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 36, i64 n <= 66, i128 n <= 136,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* behaviour below offset is irrelevant */

    /* Determine the row r containing the nth element */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Static storage for computed rows */
    static seq_t **rows = NULL;
    static size_t max_row = 0;      /* number of rows currently stored */

    /* Grow rows up to required row r */
    while (max_row < (size_t)r) {
        size_t new_row = max_row + 1;           /* 1‑based row number */
        seq_t *row = (seq_t *)malloc(new_row * sizeof(seq_t));
        if (!row) abort();                      /* allocation failure */

        if (new_row == 1) {
            /* Base case T(1,1) = 1 */
            row[0] = 1;
        } else {
            /* Compute factor = 2^(new_row-1) - 1 */
            seq_t factor = pow2((idx_t)(new_row - 1)) - 1;

            seq_t *prev = rows[max_row - 1];    /* previous row (size new_row-1) */
            for (size_t k = 1; k <= new_row; ++k) {
                seq_t term1 = (k > 1) ? prev[k - 2] : 0;
                seq_t term2 = (k <= new_row - 1) ? prev[k - 1] : 0;
                row[k - 1] = term1 - factor * term2;
            }
        }

        rows = (seq_t **)realloc(rows, (max_row + 1) * sizeof(seq_t *));
        if (!rows) abort();          /* allocation failure */
        rows[max_row] = row;
        ++max_row;
    }

    /* Compute the position within the r‑th row */
    idx_t cum_prev = (idx_t)((r - 1) * r / 2);    /* elements before row r */
    idx_t pos = n - cum_prev - 1;                /* 0‑based index in row r */
    return rows[r - 1][pos];
}
