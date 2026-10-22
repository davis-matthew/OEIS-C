/*
 * A369527  Array read by downward antidiagonals: A(n,k) = (k+1)^2*A(n-1,k) +
 *          A(n-1,k+1) with A(0,k) = 1, n >= 0, k >= 0.
 * https://oeis.org/A369527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:
   A(0,k) = 1
   A(n,k) = (k+1)^2 * A(n-1,k) + A(n-1,k+1)      for n>=1, k>=0
   Sequence a(n) is A(i,d-i) where
       d satisfies T(d) <= n < T(d+1)   with T(d)=d*(d+1)/2
       i = n - T(d)
*/
seq_t a(idx_t n)
{
    /* Cached table of A-values */
    static seq_t *A = NULL;
    static idx_t rows = 0;   /* number of rows stored   (dmax+1)   */
    static idx_t cols = 0;   /* number of columns per row (rows+1) */

    /* Find diagonal d such that T(d) <= n < T(d+1) */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Ensure the table is large enough for this diagonal */
    idx_t need_rows = d + 1;   /* rows needed up to d inclusive */
    idx_t need_cols = d + 2;   /* columns needed up to d+1               */
    if (need_rows > rows) {
        /* Rebuild the entire table up to need_rows-1 */
        rows = need_rows;
        cols = need_cols;

        seq_t *buf = (seq_t *)calloc(rows * cols, sizeof(seq_t));
        if (!buf)
            return (seq_t)0;  /* unlikely, but avoid crash */

        /* Base row A[0][k] = 1 for all k */
        for (idx_t k = 0; k < cols; ++k)
            buf[k] = (seq_t)1;

        /* Compute rows 1 .. rows-1 */
        for (idx_t r = 1; r < rows; ++r) {
            idx_t base  = r * cols;
            idx_t prev  = (r - 1) * cols;
            for (idx_t k = 0; k < rows; ++k) {   /* k up to rows-1 (=d) */
                seq_t mul = (seq_t)(k + 1) * (k + 1);
                buf[base + k] = mul * buf[prev + k] + buf[prev + (k + 1)];
            }
        }

        /* Replace old buffer */
        if (A)
            free(A);
        A = buf;
    }

    /* Compute a(n) = A[i][k]  with  i = n - T(d),  k = d - i   */
    idx_t i = n - d * (d + 1) / 2;  /* 0 <= i <= d       */
    idx_t k = d - i;                /* 0 <= k <= d       */
    return A[i * cols + k];
}
