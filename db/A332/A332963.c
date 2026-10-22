/*
 * A332963  Number triangle where T(2n,0)=T(2n,2n)=1, T(2n+1,0)=T(2n+1,2n+1)=2
 *          for all n >= 0, and the interior numbers are defined recursively
 *          by T(n,k) = (T(n-1,k-1)*T(n-1,k)+1)/T(n-2,k-1) for n > 2, 0 < k <=
 *          n.
 * https://oeis.org/A332963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * T(0,0)=1; * T(2n,0)=T(2n,2n)=1; * T(2n+1,0)=T(2n+1,2n+1)=2; *
 *            T(n,k) = (T(n-1,k-1)*T(n-1,k)+1)/T(n-2,k-1) for n>=2, 0<k<=n. *
 *            Sequence a(n) is the triangle flattened row‑wise
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Row 0 */
    seq_t *row_prevprev = (seq_t *)malloc(sizeof(seq_t));
    if (!row_prevprev) return 0;
    row_prevprev[0] = 1;

    /* Row 1 */
    seq_t *row_prev = (seq_t *)malloc(2 * sizeof(seq_t));
    if (!row_prev) { free(row_prevprev); return 0; }
    row_prev[0] = 2;
    row_prev[1] = 2;

    idx_t start = 0; /* index of first element in the current row */

    /* Check row 0 */
    if (n < start + 1) {
        seq_t result = row_prevprev[0];
        free(row_prevprev);
        free(row_prev);
        return result;
    }
    start += 1;   /* after row 0 */

    /* Check row 1 */
    if (n < start + 2) {
        seq_t result = row_prev[n - start];
        free(row_prevprev);
        free(row_prev);
        return result;
    }
    start += 2;   /* after row 1 */

    /* Generate rows from 2 onwards */
    for (idx_t r = 2; ; ++r) {
        size_t len = (size_t)(r + 1);
        seq_t *row_cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row_cur) { free(row_prevprev); free(row_prev); return 0; }

        /* borders */
        if ((r & 1) == 0) {           /* even row */
            row_cur[0] = 1;
            row_cur[len - 1] = 1;
        } else {                       /* odd row */
            row_cur[0] = 2;
            row_cur[len - 1] = 2;
        }

        /* interior entries */
        for (size_t k = 1; k + 1 < len; ++k) {
            seq_t a = row_prev[k - 1];
            seq_t b = row_prev[k];
            seq_t d = row_prevprev[k - 1];
            seq_t num = a * b + 1;
            row_cur[k] = num / d;
        }

        /* Does the requested index fall in this row? */
        if (n < start + (idx_t)len) {
            seq_t result = row_cur[n - start];
            free(row_prevprev);
            free(row_prev);
            free(row_cur);
            return result;
        }

        /* shift rows for next iteration */
        free(row_prevprev);
        row_prevprev = row_prev;
        row_prev = row_cur;
        start += (idx_t)len;
    }
}
