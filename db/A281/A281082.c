/*
 * A281082  Expansion of Product_{k>=0} (1 + x^(2*k*(k+1)+1)).
 * https://oeis.org/A281082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct centered square
 *            numbers (1,5,13,25,41,61,...) known as centered square numbers
 *            A001844. Implementation uses a standard 0/1 knapsack DP:
 *            dp[0]=1; for each centered square number c≤n, for j from n down
 *            to c do dp[j] += dp[j-c]. This computes the coeffi...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;   /* allocation failure – return 0 as safe fallback */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Enumerate centered square numbers c = 2*k*k - 2*k + 1 for k>=1 */
    for (idx_t k = 1; ; ++k) {
        idx_t c = 2 * k * k - 2 * k + 1;   /* arithmetic in idx_t avoids overflow */
        if (c > n) break;

        for (idx_t j = n; j >= c; --j) {
            dp[j] += dp[j - c];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
