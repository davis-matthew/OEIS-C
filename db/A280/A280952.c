/*
 * A280952  Expansion of Product_{k>=0} 1/(1 - x^(5*k*(k+1)/2+1)).
 * https://oeis.org/A280952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into centered pentagonal
 *            numbers (parts of the form 5*k*(k+1)/2 + 1, k >= 0)
 * Range:     u8 n <= 121, u16 n <= 451, u32 n <= 2071, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP table.  In C we use calloc (needs a cast), in C++ we use new[]. */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[(size_t)n + 1];
#else
    dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;                     /* allocation failure -> 0 */
#endif

    dp[0] = 1;                     /* one way to partition zero */

    /* Generate all centered pentagonal numbers <= n.  The count is < 64 for n <= 10000. */
    idx_t parts[128];
    size_t parts_sz = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t val = 5 * k * (k + 1) / 2 + 1;
        if (val > n) break;
        parts[parts_sz++] = val;
    }

    /* Classic unbounded coin change DP. */
    for (size_t pi = 0; pi < parts_sz; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[n];

    /* Free the table. */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
