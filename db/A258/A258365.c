/*
 * A258365  Sum over all partitions lambda of n into 10 distinct parts of
 *          Product_{i:lambda} prime(i).
 * https://oeis.org/A258365
 *
 * Offset:    55    (a(55) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all partitions of n into 10 distinct parts of
 *            the product over each part i of the i‑th prime. If n < 55 the
 *            result is 0
 * Range:     u8 none, u16 none, u32 none, u64 n <= 120, u128 n <= 832,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* The sequence only starts at n = 55. */
    if (n < 55)
        return (seq_t)0;

    /* Number of parts in each partition. */
    const int K = 10;

    /* Conservative upper bound for the value of the n‑th prime.
       p_n < n (log n + log log n) for n ≥ 6; for our limits
       n ≤ 10000 a simple n * 15 + 100 is more than enough. */
    idx_t limit = n * 15 + 100;

    /* Generate all primes up to 'limit'.  The resulting array
       contains primes in increasing order. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Ensure we have at least n primes.  This should hold for
       any n ≥ 55. */
    if (prime_cnt < (size_t)n) {
        free(primes);
        return (seq_t)0;
    }

    /* DP table: dp[k][s] is the sum of products for subsets of
       size k that have total sum s.  We store it in a flat array
       of size (K+1) * (n+1).  Indexing: pos = k * rowsize + s. */
    size_t rowsize = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc((size_t)(K + 1) * rowsize, sizeof(seq_t));
    if (!dp) {
        free(primes);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;  /* dp[0][0] = 1, all others 0 */

    /* Main DP loop over prime indices i = 1 … n.  The i‑th prime
       value is primes[i-1].  We iterate k from K down to 1
       and s from n down to i to avoid reusing a prime twice. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)primes[(size_t)i - 1];
        int max_k = (i < (idx_t)K) ? (int)i : K;
        for (int k = max_k; k >= 1; --k) {
            size_t base_k   = (size_t)k * rowsize;
            size_t base_km1 = (size_t)(k - 1) * rowsize;
            /* Use signed int for the descending loop over s. */
            for (int s = (int)n; s >= (int)i; --s) {
                size_t pos_k   = base_k   + (size_t)s;
                size_t pos_km1 = base_km1 + (size_t)(s - i);
                dp[pos_k] += dp[pos_km1] * p;
            }
        }
    }

    seq_t result = dp[(size_t)K * rowsize + (size_t)n];

    free(dp);
    free(primes);
    return result;
}
