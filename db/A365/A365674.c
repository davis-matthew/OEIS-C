/*
 * A365674  Triangle read by rows. T(n, k) = ((n - k + 1)*(n - k + 2)/2) *
 *          T(n, k - 1) + T(n - 1, k) for 0 < k < n, T(n, 0) = 1 and T(n, n) =
 *          T(n, n - 1) for n > 0.
 * https://oeis.org/A365674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row,col) entry of the triangle * defined by
 *            T(n,0)=1, T(n,k)=((n-k+1)*(n-k+2)/2)*T(n,k-1)+T(n-1,k) * for
 *            0<k<n, and T(n,n)=T(n,n-1). The sequence is read by rows *
 *            (a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The function works for any idx_t n (up to 64‑bit).  It uses only
 * idx_t arithmetic to locate the row and column, and seq_t arithmetic
 * to compute the triangle values.  Memory is allocated for each row
 * but freed before returning.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that  r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position k within the row (0 ≤ k ≤ r)   */
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate row 0:  T(0,0)=1   */
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t));
    if (!prev) return (seq_t)0;          /* allocation failure */
    prev[0] = (seq_t)1;

    /* Build subsequent rows up to r   */
    for (idx_t row = 1; row <= r; ++row) {
        seq_t *cur = (seq_t*)malloc((row + 1) * sizeof(seq_t));
        if (!cur) { free(prev); return (seq_t)0; }

        cur[0] = (seq_t)1;                        /* T(row,0)=1 */

        /* Compute 1 ≤ k < row   */
        for (idx_t kk = 1; kk < row; ++kk) {
            seq_t factor = (seq_t)(row - kk + 1) * (seq_t)(row - kk + 2);
            factor /= 2;                          /* divide by 2 */
            cur[kk] = factor * cur[kk - 1] + prev[kk];
        }

        cur[row] = cur[row - 1];                  /* T(row,row)=T(row,row-1) */

        free(prev);
        prev = cur;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}
