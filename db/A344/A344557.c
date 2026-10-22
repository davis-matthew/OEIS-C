/*
 * A344557  Triangle read by rows, T(n, k) = 2^(n - k)*M(n, k, 1/2, 1/2),
 *          where M(n, k, x, y) is a generalized Motzkin recurrence. T(n, k)
 *          for 0 <= k <= n.
 * https://oeis.org/A344557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened reading of the triangle T(n,k) defined by
 *            the recurrence: T(0,0) = 1 T(n,0) = T(n-1,0) + 4*T(n-1,1) (n ≥
 *            1) T(n,n) = 1 (n ≥ 0) T(n,k) = T(n-1,k-1) + T(n-1,k) +
 *            4*T(n-1,k+1) (1 ≤ k ≤ n-1, n ≥ 1) The sequence a(n) lists the
 *            values of T row by row: [0] 1 [1] 1, 1 [2] 5, 2, 1 [3...
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t k = n - r * (r + 1) / 2;   /* column within the row */

    /* One element row */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate two rows of size r+2 to avoid bounds checks on the
       rightmost element (which is 0 for interior calculations). */
    size_t sz = r + 2;
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !curr) {          /* minimal safety check */
        /* In practice, a failure here is fatal for the sequence. */
        /* Returning 0 to satisfy the compiler, though this case
           should not occur under normal constraints. */
        return (seq_t)0;
    }

    /* Initial row (n=0) */
    for (size_t i = 0; i < sz; ++i) {
        prev[i] = (seq_t)0;
    }
    prev[0] = (seq_t)1;

    /* Build rows up to r */
    for (idx_t row = 1; row <= r; ++row) {
        /* Clear current row */
        for (size_t i = 0; i < sz; ++i) {
            curr[i] = (seq_t)0;
        }

        /* k = 0 case: T(n,0) = T(n-1,0) + 4*T(n-1,1) */
        curr[0] = prev[0] + (seq_t)4 * prev[1];

        /* Interior columns */
        for (idx_t j = 1; j < row; ++j) {
            curr[j] = prev[j - 1] + prev[j] + (seq_t)4 * prev[j + 1];
        }

        /* k = row case: T(n,n) = 1 */
        curr[row] = (seq_t)1;

        /* Swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Result is the element at column k in row r (stored in prev). */
    seq_t result = prev[k];

    /* Free allocated memory */
    free(prev);
    free(curr);
    return result;
}
