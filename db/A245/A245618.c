/*
 * A245618  Triangle {H(n,k)} similar to Pascal's with sides of 1's, but
 *          interior entries are obtained by the rule: H(n,k) =
 *          |H(n-1,k)+(-1)^m(n,k)*H(n-1,k-1)|, where m(n,k) = H(n-1,k) +
 *          H(n-1,k-1).
 * https://oeis.org/A245618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = H(r,k) where r and k are derived from n: H(0,0)=1;
 *            H(r,0)=H(r,r)=1; for 0<k<r,
 *            H(r,k)=|H(r-1,k)+(-1)^(H(r-1,k)+H(r-1,k-1))*H(r-1,k-1)|. The
 *            sequence is obtained by listing the triangle row by row
 * Range:     u8 n <= 111, u16 n <= 310, u32 n <= 965, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached triangle rows.  rows[i] points to an array of length i+1. */
static seq_t **rows = NULL;          /* Pointer array for rows */
static size_t rows_cap = 0;          /* Allocated capacity for rows array */
static size_t row_count = 0;         /* Number of rows that have been computed */

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th element.  */
    idx_t r = 0;
    /* (r+1)*(r+2)/2 is the number of elements up to row r inclusive. */
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Grow the rows array if necessary.  */
    if (rows == NULL) {
        rows_cap = 16;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
    }

    /* Compute rows up to r.  */
    while (row_count <= (size_t)r) {
        size_t idx = row_count;   /* New row index = idx */
        size_t len = idx + 1;     /* Length of the new row */

        /* Ensure capacity for the rows pointer array. */
        if (row_count >= rows_cap) {
            rows_cap *= 2;
            rows = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
        }

        /* Allocate the new row. */
        seq_t *new_row = (seq_t *)malloc(len * sizeof(seq_t));
        new_row[0] = 1;
        new_row[idx] = 1; /* last element */

        if (idx > 0) {
            seq_t *prev = rows[idx - 1]; /* previous row */
            for (size_t k = 1; k < idx; ++k) {
                seq_t a_val = prev[k];
                seq_t b_val = prev[k - 1];
                seq_t m = a_val + b_val;                 /* m = H(r-1,k)+H(r-1,k-1) */
                if ((m & 1) == 0) {                     /* m even => + */
                    new_row[k] = a_val + b_val;
                } else {                                 /* m odd => - then abs */
                    new_row[k] = a_val > b_val ? a_val - b_val : b_val - a_val;
                }
            }
        }

        rows[row_count++] = new_row;
    }

    /* Compute the index within the row. */
    idx_t start = r * (r + 1) / 2; /* number of elements before row r */
    idx_t k = n - start;           /* column index within row r */

    return rows[r][k];
}
