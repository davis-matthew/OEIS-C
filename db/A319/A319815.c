/*
 * A319815  Number of partitions of n into exactly five positive triangular
 *          numbers.
 * https://oeis.org/A319815
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 477, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A319815
 * Number of partitions of n into exactly five positive triangular numbers.
 * Computed by dynamic programming (coin‑change style) counting
 * combinations of up to 5 triangular numbers that sum to n.
 * Time complexity: O(n√n), Memory: O(n).
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 5. */
    if (n < 5)
        return 0;

    const idx_t N = n + 1;                 /* number of sum indices 0..n   */
    const size_t total = 6 * (size_t)N;    /* 0..5 parts × 0..n sums       */

    /* Allocate a flat DP array: dp[parts * N + sum]. */
    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp)
        return 0;                          /* allocation failed – return 0 */

    /* Initialise all entries to zero. */
    for (size_t i = 0; i < total; ++i)
        dp[i] = 0;

    dp[0] = 1; /* exactly one way to obtain sum 0 with 0 parts */

    /* Enumerate all triangular numbers t(t+1)/2 ≤ n. */
    for (idx_t t = 1; ; ++t) {
        idx_t tri = t * (t + 1) / 2;          /* triangular value (idx_t) */
        if (tri > n)
            break;

        for (int parts = 1; parts <= 5; ++parts) {
            for (idx_t s = tri; s <= n; ++s) {
                dp[parts * (size_t)N + (size_t)s] +=
                    dp[(parts - 1) * (size_t)N + (size_t)(s - tri)];
            }
        }
    }

    /* The desired value is the number of ways to reach sum n
       using exactly 5 triangular numbers. */
    seq_t ans = dp[5 * (size_t)N + (size_t)n];
    free(dp);
    return ans;
}
