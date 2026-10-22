/*
 * A383235  Triangle read by rows: T(n,k) = 2*floor(k/2)*T(n-1,k) +
 *          T(n-1,k-1), 0 <= k <= n.
 * https://oeis.org/A383235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A383235: T(n,k) = 2*Floor(k/2)*T(n-1,k) + T(n-1,k-1).
   a(n) is the flattened sequence of the triangle, read row by row.
   Initial condition: T(0,0)=1, T(0,k>0)=0. */

#include <stdlib.h>
#include <stddef.h>

/* Compute the n-th term of the flattened triangle A383235. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Offset within the row */
    idx_t offset = n - (r * (r + 1)) / 2;

    /* Handle trivial cases */
    if (r == 0)            /* a(0) = T(0,0) = 1 */
        return (seq_t)1;

    /* Allocate two rows of length r+1 */
    idx_t len = r + 1;
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failure – return 0, though this should not happen for small r */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base row 0: T(0,0) = 1 */
    prev[0] = (seq_t)1;

    /* Compute rows 1 .. r */
    for (idx_t row = 1; row <= r; ++row) {
        curr[0] = (seq_t)0;   /* T(row,0) = 0 for all row > 0 */

        for (idx_t k = 1; k <= row; ++k) {
            seq_t term1 = prev[k - 1];          /* T(row-1, k-1) */
            seq_t term2 = (k <= row - 1) ? prev[k] : (seq_t)0; /* T(row-1, k) */
            seq_t coeff = (seq_t)2 * (seq_t)(k >> 1);          /* 2*floor(k/2) */
            curr[k] = term2 * coeff + term1;
        }

        /* Prepare for next iteration */
        if (row != r) {
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }
    }

    seq_t result = curr[offset];
    free(prev);
    free(curr);
    return result;
}
