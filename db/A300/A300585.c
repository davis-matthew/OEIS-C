/*
 * A300585  Number of partitions of n into squarefree parts that do not divide
 *          n.
 * https://oeis.org/A300585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Number of partitions of n into squarefree parts
   that do not divide n.  Standard coin–change DP is used,
   where for each n the coin set consists of all squarefree numbers
   not dividing n.  Complexity: O(n^2) per call. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate dp array */
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!dp)
        return (seq_t)0;  /* fallback on allocation failure */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Process each possible squarefree coin m (except those dividing n) */
    for (idx_t m = 2; m <= n; ++m)    /* skip m = 1, since 1 always divides n */
    {
        if (!is_squarefree((seq_t)m))
            continue;
        if ((n % m) == 0)
            continue;

        for (idx_t k = m; k <= n; ++k)
            dp[k] += dp[k - m];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
