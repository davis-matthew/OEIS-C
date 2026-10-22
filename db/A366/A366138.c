/*
 * A366138  Triangle read by rows. T(n, k) = A000326(n - k + 1) * T(n, k - 1)
 *          + T(n - 1, k) for 0 < k < n. T(n, 0) = 1 and T(n, n) = T(n, n - 1)
 *          if n > 0.
 * https://oeis.org/A366138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pentagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition (OEIS A366138):
   The triangle T(n,k) satisfies
     T(n,0)   = 1,
     T(n,n)   = T(n,n-1)  (n>0),
     T(n,k)   = P(n-k+1) * T(n,k-1) + T(n-1,k)  for 0<k<n,
   where P(m) = m*(3*m-1)/2 (the pentagonal numbers).
   The sequence a(n) is the triangle read by rows:
     a(0)=T(0,0),
     a(1)=T(1,0), a(2)=T(1,1),
     a(3)=T(2,0), a(4)=T(2,1), a(5)=T(2,2),
     …
   The first few terms are 1,1,1,1,6,6,1,18,96,96,1,40,576,2976,2976,1,...
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pentagonal.h"

seq_t a(idx_t n)
{
    /* Find the row r such that prefix[r] = (r+1)(r+2)/2 > n. */
    idx_t low  = 0;
    idx_t high = n;          /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t pref = ((seq_t)(mid + 1) * (mid + 2)) / 2;
        if (pref > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                          /* target row */
    idx_t prefix_prev = r * (r + 1) / 2;    /* terms before row r */
    idx_t c = n - prefix_prev;              /* column index in row r */

    /* Allocate rows iteratively. */
    seq_t *prev = NULL;
    seq_t *curr = NULL;

    for (idx_t i = 0; i <= r; ++i) {
        curr = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        curr[0] = 1;                      /* T(i,0) = 1 */

        if (i > 0) {
            /* Compute inner entries for 1 <= k < i. */
            for (idx_t k = 1; k < i; ++k) {
                seq_t weight = pentagonal(i - k + 1);
                curr[k] = weight * curr[k - 1] + prev[k];
            }
            /* Last entry equals previous one. */
            curr[i] = curr[i - 1];
        }

        /* If we reached the target row, return the requested element. */
        if (i == r) {
            seq_t result = curr[c];
            free(curr);
            if (prev) free(prev);
            return result;
        }

        /* Prepare for next iteration. */
        if (prev) free(prev);
        prev = curr;
    }

    /* Should never reach here. */
    return 0;
}
