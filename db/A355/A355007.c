/*
 * A355007  Triangle read by rows. T(n, k) = n^k * |Stirling1(n, k)|.
 * https://oeis.org/A355007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A355007 is the triangle T(n,k)=n^k * |Stirling1(n,k)| read
   by rows.  For a given linear index n (zero‑based) we first find the
   corresponding row r and column c, then return
   T(r,c)= r^c * |Stirling1(r,c)|.
   The unsigned Stirling numbers of the first kind are generated with
   the recurrence
       S(n,k)=S(n-1,k-1)+(n-1)*S(n-1,k),
   which holds for the absolute values.
   Complexity: O(r^2) arithmetic with r≈√(2n); for n≤10000 r≤141. */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Determine the row (r) and column (c) of the n‑th
       element when the triangle is read by rows. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t c = n - ((unsigned __int128)r * (r + 1)) / 2;

    /* Allocate array to hold one row of Stirling numbers. */
    seq_t *S = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!S) return (seq_t)0;   /* allocation failure → fallback to 0 */

    /* Initialise for row 0 : S[0] = 1 */
    for (idx_t i = 0; i <= r; ++i) S[i] = 0;
    S[0] = 1;

    /* Build rows 1 … r using the unsigned recurrence. */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t k = row; k >= 1; --k) {
            S[k] = S[k - 1] + (seq_t)(row - 1) * S[k];
        }
        S[0] = 0;   /* S(row,0)=0 for all r>0 */
    }

    /* Compute r^c */
    seq_t pow = 1;
    for (idx_t i = 0; i < c; ++i)
        pow *= (seq_t)r;

    /* Result T(r,c) = r^c * S[r][c] */
    seq_t result = pow * S[c];

    free(S);
    return result;
}
