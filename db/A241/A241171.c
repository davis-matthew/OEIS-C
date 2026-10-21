/*
 * A241171  Triangle read by rows: Joffe's central differences of zero,
 *          T(n,k), 1 <= k <= n.
 * https://oeis.org/A241171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 32, u64 n <= 74, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A241171 – Joffe's central differences of zero (triangle read by rows)
 *  a(n) = T(r,c)
 *  r = smallest integer with r*(r+1)/2 >= n          (row number)
 *  c = n - r*(r-1)/2                                 (column in that row)
 *  T(1,1) = 1
 *  T(n,k) = 0  if k<=0 or k>n
 *  T(n,1) = 1
 *  T(n,k) = k*(2*k-1)*T(n-1,k-1) + k*k*T(n-1,k)     (n>=2, k>=2)
 *
 *  The function computes only as many triangle rows as required for the
 *  requested n.  The implementation is completely self‑contained and
 *  works for any builds that define seq_t and idx_t.  No external
 *  utilities are invoked.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Determine the row number r and column c that correspond to index n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;          /* r*(r+1)/2 < n  → r++ */
    idx_t c = n - (r - 1) * r / 2;            /* column within row r */

    /* Allocate two rows for dynamic programming.  Length r+1 is enough for the
       largest needed row.  Indices start at 1 (index 0 is unused).       */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));

    /* Initialise previous row to zeros.                           */
    for (idx_t i = 0; i <= r; ++i) prev[i] = 0;

    /* Build rows incrementally until we reach row r.              */
    for (idx_t row = 1; row <= r; ++row) {
        /* clear current row */
        for (idx_t i = 0; i <= r; ++i) curr[i] = 0;

        /* compute entries of current row */
        for (idx_t k = 1; k <= row; ++k) {
            if (k == 1) {
                curr[k] = 1;
            } else {
                /* compute k*(2*k-1) and k*k as seq_t to avoid overflows
                   when seq_t is an arbitrary‑precision type.              */
                seq_t m1 = (seq_t)(k * (2 * k - 1));
                seq_t m2 = (seq_t)(k * k);
                curr[k] = prev[k - 1] * m1 + prev[k] * m2;
            }
        }

        /* if we have reached the desired row, extract the answer */
        if (row == r) {
            seq_t result = curr[c];
            free(prev);
            free(curr);
            return result;
        }

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Should never reach here.                                   */
    free(prev);
    free(curr);
    return 0;
}
