/*
 * A393989  Expansion of g.f.: Product_{k>=1} (1 + x^(k*(k+5)/2)).
 * https://oeis.org/A393989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            k*(k+5)/2
 * Range:     u8 n <= 87, u16 n <= 1364, u32 n <= 5654, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for all n >= 0.  The function allocates a small DP array
 * of size n+1, performs a 0/1 knapsack with all parts k*(k+5)/2 <= n,
 * and returns the number of partitions.  It is correct for any
 * seq_t/idx_t combination supported by the build. */
seq_t a(idx_t n)
{
    /* Allocate DP array and initialize. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) exit(1);           /* memory allocation failure */
    dp[0] = 1;                  /* empty subset counts as 1 */

    /* Enumerate all allowed parts k*(k+5)/2 up to n. */
    for (idx_t k = 1; ; ++k) {
        seq_t part = ((seq_t)k * (k + 5)) / 2;
        if (part > n) break;
        idx_t p = (idx_t)part;
        /* 0/1 knapsack update: add this part to all sums j >= p. */
        for (idx_t j = n; j >= p; --j)
            dp[j] += dp[j - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
