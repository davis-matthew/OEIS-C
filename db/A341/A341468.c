/*
 * A341468  Number of partitions of n into 9 distinct nonprime parts.
 * https://oeis.org/A341468
 *
 * Offset:    79    (a(79) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 9 distinct nonprime parts
 *            (a part may be 1 or a composite number). That is, a(n) counts
 *            9‑tuples of distinct nonprime integers whose sum equals n. The
 *            sequence starts at n = 79; the function returns the correct
 *            value for all n ≥ 79
 * Range:     u8 n <= 106, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* allocate DP tables */
    seq_t *dp[10];
    for (int k = 0; k <= 9; ++k) {
        dp[k] = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!dp[k]) {
            for (int j = 0; j < k; ++j) free(dp[j]);
            return 0;
        }
    }

    dp[0][0] = 1;   /* one way to sum to 0 with 0 parts */

    /* iterate over all numbers up to n, keeping only nonprime ones */
    for (idx_t v = 1; v <= n; ++v) {
        if (v != 1 && is_prime(v)) continue;   /* skip primes, keep 1 */

        /* update DP tables in reverse order to preserve distinctness */
        for (int k = 9; k >= 1; --k) {
            for (idx_t s = n; s >= v; --s) {
                if (dp[k - 1][s - v]) {
                    dp[k][s] += dp[k - 1][s - v];
                }
            }
        }
    }

    seq_t result = dp[9][n];

    /* free memory */
    for (int k = 0; k <= 9; ++k) free(dp[k]);

    return result;
}
