/*
 * A326455  Sum of all the parts in the partitions of n into 8 primes.
 * https://oeis.org/A326455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A259198(n) where A259198(n) = number of partitions of
 *            n into 8 primes. The implementation uses a classic coin‑change
 *            DP over the list of primes ≤ n, counting combinations
 *            (unordered) of exactly 8 primes that sum to n. The result is
 *            multiplied by n
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gen_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gen_primes_upto.h"

/* ----------------------------------------------------------------- */
/* Generate all primes up to limit (inclusive) into an array.          */
/* On success out points to a malloc'd array of size *cnt.  Caller    */
/* must free(out).  If no primes or allocation fails, *out = NULL.   */

/* ----------------------------------------------------------------- */
/* Main sequence function.                                            */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 16) /* 2*8 = 16 is the smallest possible sum of 8 primes */
        return 0;

    /* Generate primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    gen_primes_upto(n, &primes, &prime_cnt);
    if (prime_cnt == 0) {
        free(primes);
        return 0;
    }

    /* DP array: dp[k][s] stored linearly, k = 0..8, s = 0..n */
    size_t stride = (size_t)n + 1;
    size_t dp_size = 9 * stride;
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) {
        free(primes);
        return 0;
    }

    dp[0] = 1; /* dp[0][0] = 1 */

    /* Coin‑change DP: iterate primes, update counts for each k and sum */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        for (idx_t sum = p; sum <= n; ++sum) {
            size_t prev_sum = (size_t)(sum - p);
            for (int k = 1; k <= 8; ++k) {
                size_t cur_idx  = (size_t)k * stride + (size_t)sum;
                size_t prev_idx = (size_t)(k - 1) * stride + prev_sum;
                dp[cur_idx] += dp[prev_idx];
            }
        }
    }

    seq_t ways = dp[8 * stride + (size_t)n];
    seq_t result = (seq_t)n * ways;

    free(dp);
    free(primes);
    return result;
}
