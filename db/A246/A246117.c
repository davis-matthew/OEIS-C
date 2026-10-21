/*
 * A246117  Number of parity preserving permutations of the set {1,2,...,n}
 *          with exactly k cycles.
 * https://oeis.org/A246117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence computation for OEIS A246117.
 * T(1,1)=1; T(n,1)=0 for n>=2; T(n,k)=0 for k>n.
 * T(n+1,k)=floor(n/2)*T(n,k)+T(n,k-1).
 * a(n) is the n‑th element in row‑major order of T(n,k) (offset 1).
 * The function builds successive rows until the requested row is reached
 * and returns the requested entry. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined below offset 1 */
        return 0;

    /* Find the row r that contains the n‑th element.
       cum = 1+2+...+r = r(r+1)/2  is the cumulative count up to row r. */
    idx_t r  = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Determine the column (1‑based) within that row. */
    idx_t prev_cum = (r - 1) * r / 2;   /* sum of 1..(r-1) */
    idx_t col = n - prev_cum;           /* 1 ≤ col ≤ r */

    /* Allocate the first row (row 1). */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!prev)
        return 0;                      /* allocation failure */
    prev[0] = 1;                        /* T(1,1)=1 */

    /* Build rows 2 through r. */
    for (idx_t row = 2; row <= r; ++row) {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * row);
        if (!curr) {                    /* allocation failure */
            free(prev);
            return 0;
        }

        curr[0] = 0;                    /* T(row,1)=0 for row>=2 */

        seq_t mult = (seq_t)((row - 1) / 2);   /* floor((row-1)/2) */

        /* Compute entries k=2..row.  previous row has length row-1. */
        for (idx_t k = 2; k <= row; ++k) {
            seq_t term_n = (k - 1) < row - 1 ? prev[k - 1] : 0;
            seq_t term_n_minus1 = prev[k - 2];
            curr[k - 1] = mult * term_n + term_n_minus1;
        }

        free(prev);
        prev = curr;
    }

    /* Extract the wanted element and free the last row. */
    seq_t result = prev[col - 1];
    free(prev);
    return result;
}
