/*
 * A282629  Sheffer triangle (exp(x), exp(3*x) - 1). Named S2[3,1].
 * https://oeis.org/A282629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 68, u64 n <= 175,
 *            u128 n <= 503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the Sheffer triangle (exp(x), exp(3*x)-1):
 *   T(0,0) = 1,
 *   T(n,0) = 1,
 *   T(n,m) = 3*T(n-1,m-1) + (1+3*m)*T(n-1,m)   (m ≥ 1).
 *   
 *  The generated sequence a(n) is the row‑wise flattening of that triangle:
 *  a(n) = T(r, c) where r is the unique integer with r*(r+1)/2 ≤ n <
 *  (r+1)*(r+2)/2 and c = n - r*(r+1)/2 (0 ≤ c ≤ r).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        r++;

    /* Compute column within that row. */
    idx_t col = n - r * (r + 1) / 2;

    /* Allocate array for the current row; length r+1. */
    seq_t *dp = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;          /* fall back to 0 on allocation failure */

    /* Base case: T(0,0) = 1. */
    dp[0] = (seq_t)1;

    /* Build rows up to r.  Each iteration updates dp in reverse
       to use only the previous row's values. */
    for (idx_t i = 1; i <= r; ++i) {
        dp[0] = (seq_t)1;             /* first column stays 1 */
        for (idx_t j = i; j >= 1; --j) {
            seq_t term1 = ((seq_t)3) * dp[j - 1];
            seq_t factor = (seq_t)(1 + 3 * j);
            seq_t term2 = factor * dp[j];
            dp[j] = term1 + term2;
        }
    }

    seq_t result = dp[col];
    free(dp);
    return result;
}
