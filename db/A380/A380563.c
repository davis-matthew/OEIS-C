/*
 * A380563  Rectangular array R read by descending antidiagonals: (row 1) =
 *          (R(1,k)) = (1 + A010060(k)), k >= 1; (row n+1) = inverse runlength
 *          sequence of row n; and R(n,1) = (1, 1, 1, 1, 1,...) = (A130196(n))
 *          for n >= 1. See Comments.
 * https://oeis.org/A380563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A380563 – Recursive “binary run-length” array.
   For n ≥ 1, a(n) is the value R(i,j) read along descending antidiagonals
   of the infinite 2‑D array R, where:
     • R(1,k) = 1 + Thue–Morse(k-1)  (∈{1,2})
     • for n ≥ 1, R(n+1,·) is the inverse run-length sequence of R(n,·)
       starting with 1, and the first column of R is constant 1.
   This implementation computes the required term directly.  The work
   per call is O(√n · √n) ≈ O(n).
*/

#include <stdint.h>
#include <stdlib.h>

/* Convert a 1‑based index n into row i and column j
   of the array read by descending antidiagonals.        */
static inline void antidiag(idx_t n, idx_t *i, idx_t *j)
{
    idx_t m = 1;
    /* Find smallest m such that m*(m+1)/2 ≥ n.          */
    while (( (__int128)m * (m + 1) ) / 2 < n) ++m;

    /* Diagonal number D = m+1 (sum of row+col indices).  */
    idx_t D = m + 1;
    /* Number of terms before this diagonal.              */
    idx_t before = ( (m - 1) * m ) / 2;
    /* 1‑based offset inside diagonal.                    */
    idx_t offset = n - before;

    *i = offset;           /* row number (1‑based) */
    *j = D - offset;       /* column number (1‑based) */
}

/* Sequence generator for A380563.  Returns a(n).          */
seq_t a(idx_t n)
{
    idx_t row, col;
    antidiag(n, &row, &col);

    /* Allocate array of pointers for rows 1..row.     */
    uint8_t **rows = (uint8_t **)malloc( (row+1) * sizeof(uint8_t *) );
    if (!rows) return (seq_t)0;           /* allocation failed */

    for (idx_t k = 1; k <= row; ++k) rows[k] = NULL;

    /* Build rows one by one, only as many columns as needed. */
    for (idx_t r = 1; r <= row; ++r) {
        size_t len = (size_t)col;          /* number of columns needed   */
        uint8_t *cur = (uint8_t *)malloc(len);
        if (!cur) {
            /* clean up allocated rows before returning */
            for (idx_t t = 1; t < r; ++t) free(rows[t]);
            free(rows);
            return (seq_t)0;
        }
        rows[r] = cur;

        if (r == 1) {
            /* Row 1: 1 + Thue–Morse(k‑1)  → 1 or 2 */
            for (size_t k = 0; k < len; ++k) {
                uint8_t v = (__builtin_parityll(k) ? 2 : 1);
                cur[k] = v;
            }
        } else {
            /* Inverse run‑length of previous row.      */
            uint8_t *prev = rows[r-1];
            size_t pos  = 0;          /* index in current row   */
            size_t p    = 0;          /* index in previous row  */
            uint8_t val  = 1;         /* current value to write */

            while (pos < len) {
                size_t run = (size_t)prev[p++];
                size_t take = run;
                if (take > len - pos) take = len - pos;
                for (size_t t = 0; t < take; ++t)
                    cur[pos++] = val;
                val = (val == 1 ? 2 : 1);   /* toggle between 1 and 2 */
            }
        }
    }

    /* The requested term is in rows[row][col‑1]. */
    uint8_t result = rows[row][col-1];

    /* Free dynamic memory. */
    for (idx_t k = 1; k <= row; ++k) free(rows[k]);
    free(rows);

    return (seq_t)result;
}
