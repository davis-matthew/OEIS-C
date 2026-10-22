/*
 * A392666  Triangle read by rows: T(n,k) (1 <= k <= n) given by T(n,1) =
 *          T(n,n) = 1, otherwise T(n,k) = (m*n-m*k+1)*T(n-1,k-1) +
 *          (m*k-m+1)*T(n-1,k), where m = -5.
 * https://oeis.org/A392666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   triangle read by rows. T(1,1)=1; for 1<k<n: T(n,k) =
 *            (1-5*(n-k))*T(n-1,k-1) + (6-5*k)*T(n-1,k). a(n) is the nth
 *            element of this triangle read row‑by‑row
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 49, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row r containing position n: r(r+1)/2 >= n */
    idx_t r = 1;
    while (((seq_t)r * (r + 1) / 2) < n) ++r;

    /* Allocate two rows of size r+1 (use index 1..r) */
    size_t sz = (size_t)(r + 1);
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* Base row: n = 1 */
    prev[1] = (seq_t)1;

    /* Build rows up to r */
    for (idx_t i = 2; i <= r; ++i) {
        cur[1] = (seq_t)1;
        cur[i] = (seq_t)1;
        for (idx_t k = 2; k < i; ++k) {
            seq_t coeff1 = (seq_t)1 - (seq_t)5 * ((seq_t)(i - k));
            seq_t coeff2 = (seq_t)6 - (seq_t)5 * ((seq_t)k);
            cur[k] = coeff1 * prev[k - 1] + coeff2 * prev[k];
        }
        /* Swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Position within the final row */
    idx_t pos = n - (idx_t)(((seq_t)(r - 1) * r) / 2);

    seq_t result = prev[pos];

    /* Free temporary arrays */
    free(prev);
    free(cur);

    return result;
}
