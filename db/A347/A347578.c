/*
 * A347578  Number of partitions of n into at most 4 prime parts.
 * https://oeis.org/A347578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 4 prime parts
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) exactly using DP over primes up to n. */

#include <stdlib.h>
#include "../utils/sieve_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* DP table dp[s][k] stored as dp[s*5 + k], k = 0..4 */
    size_t sz = (size_t)(n + 1) * 5;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return (seq_t)0;

    /* initialise to zero, then dp[0][0] = 1 */
    for (size_t i = 0; i < sz; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* dp[0*5 + 0] */

    /* generate all primes <= n */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(n, &primes, &prime_cnt);

    /* dynamic programming over primes */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        for (idx_t s = p; s <= n; ++s) {
            size_t base   = (size_t)s * 5;
            size_t baseP  = (size_t)(s - p) * 5;
            for (int k = 1; k <= 4; ++k) {
                dp[base + k] += dp[baseP + (k - 1)];
            }
        }
    }

    /* sum partitions with 1..4 parts */
    seq_t result = (seq_t)0;
    for (int k = 1; k <= 4; ++k) {
        result += dp[(size_t)n * 5 + k];
    }

    /* free resources */
    free(dp);
    free(primes);

    return result;
}
