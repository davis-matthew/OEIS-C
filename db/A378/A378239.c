/*
 * A378239  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,0) = 0^n and T(n,k) = k * Sum_{r=0..n}
 *          binomial(n,r) * binomial(2*n+2*r+k,n)/(2*n+2*r+k) for k > 0.
 * https://oeis.org/A378239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   T(0,k) = 1                         (k ≥ 0)
   T(n,0) = 0                         (n > 0)
   T(n,k) = T(n,k-1) + T(n-1,k+1) + T(n-1,k+3)   (n > 0, k > 0)

   a(n) is the value T(n',k') obtained by reading the square by antidiagonals
   (first reading (0,0), then (0,1),(1,0), then (0,2),(1,1),(2,0), and so on).
   Initial values: a(0)=1, a(1)=1, a(2)=0.  */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the antidiagonal index s such that
       s(s+1)/2 ≤ n < (s+1)(s+2)/2.  */
    idx_t s = 0;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* Compute (row,column) of the desired cell.  */
    unsigned __int128 diag_start128 = (unsigned __int128)s * (s + 1) / 2;
    idx_t diag_start = (idx_t)diag_start128;
    idx_t offset   = n - diag_start;  /* n = row + column   */
    idx_t n_row    = offset;          /* row index           */
    idx_t k_col    = s - offset;      /* column index        */

    /* T(0,k)=1 for all k.  If the cell is in row 0 we can return immediately. */
    if (n_row == 0) {
        return (seq_t)1;
    }

    /* The recursion can reach up to k_target + 3*n_row in row 0.  */
    idx_t N          = n_row;
    idx_t K_target   = k_col;
    idx_t kmax0 = K_target + 3 * N;   /* maximal k needed in row 0 */

    /* Allocate and initialise row 0.  */
    seq_t *prev = (seq_t *)malloc((size_t)(kmax0 + 1) * sizeof(seq_t));
    if (!prev) return (seq_t)0;  /* fallback on allocation failure */
    for (idx_t i = 0; i <= kmax0; ++i) {
        prev[i] = (seq_t)1;
    }

    /* Compute rows 1 .. N.  */
    for (idx_t i = 1; i <= N; ++i) {
        idx_t kmax_i = K_target + 3 * (N - i);   /* maximal k in this row */
        seq_t *cur = (seq_t *)malloc((size_t)(kmax_i + 1) * sizeof(seq_t));
        if (!cur) { free(prev); return (seq_t)0; }  /* allocation failure */

        cur[0] = (seq_t)0;  /* T(n,0)=0 for n>0 */
        for (idx_t k = 1; k <= kmax_i; ++k) {
            /* cur[k] = cur[k-1] + prev[k+1] + prev[k+3];   */
            cur[k] = cur[k - 1] + prev[k + 1] + prev[k + 3];
        }

        free(prev);
        prev = cur;  /* next iteration uses this as the previous row */
    }

    seq_t result = prev[K_target];
    free(prev);
    return result;
}
