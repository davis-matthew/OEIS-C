/*
 * A399683  Triangle read by rows: T(n,k) is the maximum number of colors that
 *          can be used in an edge-coloring of K_n containing no rainbow copy
 *          of the k-vertex path P_k, for 3 <= k <= n.
 * https://oeis.org/A399683
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 301, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   solve_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements OEIS sequence A399683.
   The sequence is the flattening of the triangle T(n,k),
   where for 3 ≤ k ≤ n

       T(n,3) = 1
       T(4,4) = 3,  T(n,4) = 2 for n ≥ 5

       for n ≥ k ≥ 5
           let m = ⌊(k‑1)/2⌋
           e = 1 if k is odd, otherwise e = 2
           term1 = binom(k‑2,2) + 1
           term2 = binom(m‑1,2) + (m‑1)*(n‑m+1) + e
           T(n,k) = max(term1, term2)
   The flattened sequence a(n) (with offset 3) lists the triangle
   row–by–row, starting with a(3)=T(3,3)=1.
*/

#include <stdint.h>
#include "../utils/solve_T.h"

seq_t a(idx_t n)
{
    /* a(3) is the first term in the flattened triangle. */
    if (n < 3) {
        return (seq_t)0;   /* undefined for n < 3 */
    }

    /* Zero‑based position within the flattened list. */
    idx_t pos = n - 3;

    /* Find the row r such that the cumulative number of
       terms up to row r-1 is ≤ pos < cumulative up to row r. */
    idx_t r = 3;        /* first valid row */
    idx_t cumulative = 0;
    while (cumulative + (r - 2) <= pos) {
        cumulative += (r - 2);
        r += 1;
    }

    /* Position inside row r.  Offsets run 0..(r-3). */
    idx_t offset = pos - cumulative;
    idx_t k = 3 + offset;   /* column index */

    return solve_T(r, k);
}
