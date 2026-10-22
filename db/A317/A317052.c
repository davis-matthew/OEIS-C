/*
 * A317052  Triangle read by rows: T(0,0) = 1; T(n,k) = 9*T(n-1,k) +
 *          T(n-2,k-1) for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) is the flattened triangle T given by * T(0,0) = 1 * T(n,k)
 *            = 9*T(n-1,k) + T(n-2,k-1), 0 ≤ k ≤ ⌊n/2⌋ * T(n,k) = 0 if n<0 or
 *            k<0 * The sequence lists all non‑zero T(n,k) in row‑major order,
 *            * i.e. first row 0, then row 1, then row 2, …
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 35, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r containing the n-th element and the
       offset k within that row.  Row r has length L = r/2 + 1. */
    idx_t r = 0;
    idx_t ran = 0;           /* number of entries processed before row r */

    while (ran + (r / 2 + 1) <= n) {
        ran += r / 2 + 1;
        r++;
    }
    idx_t k = n - ran;       /* position inside row r */

    /* Temporary storage for two previous rows.  Each row is a
       variable‑length array of seq_t. */
    seq_t *row_prev2 = NULL; /* row r-2 */
    seq_t *row_prev1 = NULL; /* row r-1 */
    seq_t *row_cur   = NULL; /* row r we're building */

    /* Build the triangle up to row r. */
    for (idx_t cur = 0; cur <= r; cur++) {
        idx_t len_cur = cur / 2 + 1;               /* length of current row   */
        row_cur = (seq_t*)malloc(len_cur * sizeof(seq_t));
        if (!row_cur) return 0; /* allocation failure – unlikely in tests */

        /* Lengths of the previous rows, if they exist. */
        idx_t len_prev1 = (cur >= 1) ? ((cur - 1) / 2 + 1) : 0;
        idx_t len_prev2 = (cur >= 2) ? ((cur - 2) / 2 + 1) : 0;

        for (idx_t i = 0; i < len_cur; i++) {
            if (cur == 0) {                       /* first row: T(0,0)=1 */
                row_cur[i] = (seq_t)1;
            } else {
                seq_t term1 = 0;
                seq_t term2 = 0;

                if (i < len_prev1)                /* T(n-1,k) */
                    term1 = row_prev1[i];
                if (i > 0 && i - 1 < len_prev2)   /* T(n-2,k-1) */
                    term2 = row_prev2[i - 1];

                row_cur[i] = 9 * term1 + term2;
            }
        }

        /* Advance the row buffers. */
        if (row_prev2) free(row_prev2);
        row_prev2 = row_prev1;
        row_prev1 = row_cur;
    }

    /* row_cur (alias row_prev1) now holds row r; fetch element k. */
    seq_t result = row_prev1[k];

    /* Clean up. */
    free(row_prev1);
    if (row_prev2) free(row_prev2);

    return result;
}
