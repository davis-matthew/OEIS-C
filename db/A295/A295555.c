/*
 * A295555  Generalized Pascal triangle read by rows: add the four terms that
 *          are right above you, three rows back.
 * https://oeis.org/A295555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 446, u32 n <= 1456, u64 n <= 5399,
 *            u128 n <= 9482, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   OEIS A295555:  Generalized Pascal triangle read by rows
   Recurrence (for n >= 3, 0 < k < n):
       T(n,0) = T(n,n) = 1
       T(2,1) = 1
       T(n,k) = T(n-3,k-3) + T(n-3,k-2) + T(n-3,k-1) + T(n-3,k)
   The sequence a(n) is the concatenation of the rows of T.
*/

#include <stdlib.h>
#include <stdbool.h>

/* extern linkage */
seq_t a(idx_t n)
{
    /* static state for on‑demand construction of rows */
    static bool initialized = false;
    static idx_t current_row = 0;              /* index of the last computed row */
    static seq_t *row_ptr[3] = {NULL, NULL, NULL}; /* cyclic storage of rows */
    static size_t row_size[3] = {0, 0, 0};     /* lengths of the rows */
    static idx_t last_len = 0;                 /* number of elements in the last row added */
    static idx_t tot_terms = 0;                /* total number of terms generated */

    /* initialise with row 0 (1) */
    if (!initialized) {
        row_ptr[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!row_ptr[0]) return (seq_t)0;     /* allocation failure */
        row_ptr[0][0] = (seq_t)1;
        row_size[0] = 1;
        current_row = 0;
        tot_terms = 1;
        initialized = true;
        if (n == 0) return row_ptr[0][0];
    }

    /* generate rows until we have produced at least n+1 terms */
    while (tot_terms <= n) {
        idx_t r = current_row + 1;            /* next row index */
        size_t len = r + 1;                   /* row length */

        /* allocate new row */
        seq_t *new_row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!new_row) return (seq_t)0;        /* allocation failure */

        for (size_t k = 0; k < len; ++k) {
            seq_t val;
            if (k == 0 || k == len - 1) {
                /* first or last entry */
                val = (seq_t)1;
            } else if (r == 2 && k == 1) {
                /* special middle cell of row 2 */
                val = (seq_t)1;
            } else if (r >= 3) {
                /* recurrence using row r-3 */
                val = (seq_t)0;
                seq_t *prev_row = row_ptr[(r - 3) % 3];
                size_t prev_len = row_size[(r - 3) % 3];
                for (size_t d = 0; d < 4; ++d) {
                    if (k >= d) {
                        size_t idx2 = k - d;
                        if (idx2 < prev_len) {
                            val += prev_row[idx2];
                        }
                    }
                }
            } else {
                /* rows 0,1,2 except the special case handled above */
                val = (seq_t)1;
            }
            new_row[k] = val;
        }

        /* free the row 3 places older than the new one */
        free(row_ptr[r % 3]);

        /* store the new row */
        row_ptr[r % 3] = new_row;
        row_size[r % 3] = len;

        current_row = r;
        tot_terms += len;
        last_len = (idx_t)len;
    }

    /* determine which row contains the desired term */
    idx_t row_start = tot_terms - last_len;          /* index of the first term of current_row */
    idx_t index_in_row = n - row_start;              /* offset within that row */

    return row_ptr[current_row % 3][index_in_row];
}
