/*
 * A392665  Triangle read by rows: T(n,k) (1 <= k <= n) given by T(n,1) =
 *          T(n,n) = 1, otherwise T(n,k) = (m*n-m*k+1)*T(n-1,k-1) +
 *          (m*k-m+1)*T(n-1,k), where m = -4.
 * https://oeis.org/A392665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th entry of a triangle read by rows. The triangle
 *            T(n,k) (1 ≤ k ≤ n) satisfies: T(n,1) = T(n,n) = 1 T(n,k) =
 *            (1-4*(n-k))*T(n-1,k-1) + (5-4*k)*T(n-1,k) for 1<k<n a(n) is
 *            obtained by flattening the rows of T: 1,1,1,1,-6,1,1,11,11,1,…
 *            This implementation runs in O(r²) time where r ≈ √(...
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of bounds, undefined for this sequence */

    /* Find the row r that contains the n-th element:
       cumulative elements up to row r = r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Allocate the previous row (row 1 initially) */
    idx_t prev_len = 1;
    seq_t *prev = (seq_t*)malloc(prev_len * sizeof(seq_t));
    if (!prev) return (seq_t)0;          /* allocation failed – unlikely */
    prev[0] = (seq_t)1;                  /* T(1,1) = 1 */

    /* Build successive rows up to row r */
    for (idx_t i = 2; i <= r; ++i) {     /* i = current row number */
        idx_t curr_len = i;
        seq_t *curr = (seq_t*)malloc(curr_len * sizeof(seq_t));
        if (!curr) { free(prev); return (seq_t)0; }

        curr[0] = (seq_t)1;             /* k = 1 */
        curr[i-1] = (seq_t)1;           /* k = i (last entry) */

        for (idx_t j = 2; j < i; ++j) { /* j is 1‑based k */
            idx_t k = j;
            seq_t left  = ((seq_t)1 - ((seq_t)4 * (seq_t)(i - k)));
            seq_t right = ((seq_t)5 - ((seq_t)4 * (seq_t)k));

            /* prev indices: k-1 -> prev[k-2],  k -> prev[k-1] */
            seq_t val = left * prev[k-2] + right * prev[k-1];
            curr[j-1] = val;
        }

        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* Determine the column k of the desired element within row r */
    idx_t prev_cumulative = (r * (r - 1)) / 2;   /* elements before row r */
    idx_t k = n - prev_cumulative;              /* 1‑based column index */
    seq_t result = prev[k-1];

    free(prev);
    return result;
}
