/*
 * A326463  Sum of the largest parts of the partitions of n into 8 primes.
 * https://oeis.org/A326463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = Sum of the largest parts of the partitions
 * of n into 8 primes.  It iterates over all primes ≤ n and uses an
 * unbounded knapsack DP to count partitions with parts bounded by the
 * current prime.  The incremental difference of the partition count
 * between successive primes gives the number of partitions whose largest
 * part equals that prime; multiplying by the prime and summing yields
 * the desired result.  The DP table has dimensions (n+1) × 9.  Complexity
 * is O(n·π(n)) – about 1–2×10⁸ elementary operations for n≈10000.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* For n < 16 there is no partition into 8 primes (2+…+2). */
    if (n < 16)
        return 0;

    /* Gather all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);
    if (!primes) return 0; /* should not happen for n ≤ 10000 */

    const size_t cols   = 9;          /* k = 0 … 8 */
    const size_t rows   = (size_t)n + 1;
    const size_t total  = rows * cols;
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp) {            /* allocation failure – unlikely for n ≤ 10000 */
        free(primes);
        return 0;
    }
    dp[0] = 1;  /* dp[0][0] = 1 */

    seq_t ans   = 0;
    seq_t prevF = 0; /* number of partitions with parts ≤ previous prime */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];

        /* Unbounded knapsack update for this prime p.  For each j ≥ p
         * and for k = 1 … 8, add partitions of j−p into k−1 primes. */
        for (idx_t j = p; j <= n; ++j) {
            size_t base = (size_t)(j - p) * cols;
            size_t cur  = (size_t)j * cols;
            for (int k = 1; k <= 8; ++k) {
                dp[cur + k] += dp[base + (k - 1)];
            }
        }

        /* Number of partitions after processing prime p. */
        seq_t newF  = dp[(size_t)n * cols + 8];
        seq_t delta = newF - prevF;   /* partitions whose largest part is p */
        ans += (seq_t)p * delta;
        prevF = newF;
    }

    free(primes);
    free(dp);
    return ans;
}
