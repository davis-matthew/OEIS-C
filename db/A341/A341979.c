/*
 * A341979  Number of partitions of n into 8 distinct primes (counting 1 as a
 *          prime).
 * https://oeis.org/A341979
 *
 * Offset:    59    (a(59) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of partitions of n into 8 distinct primes, *
 *            where 1 is considered a prime. The function uses a simple DP
 *            over all * primes ≤ n (with 1 inserted at the beginning)
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* The OEIS offset is 59. For n < 59 we return 0 (value is irrelevant). */
    if (n < 59) {
        return 0;
    }

    /* Build all true primes ≤ n. */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Allocate an array that contains 1 (treated as a prime) followed by
     * the true primes. */
    size_t total_cnt = prime_cnt + 1;
    uint64_t *primes = (uint64_t*)malloc(sizeof(uint64_t) * total_cnt);
    if (!primes) {
        free(prime_list);
        return 0;
    }
    primes[0] = 1;
    memcpy(primes + 1, prime_list, sizeof(uint64_t) * prime_cnt);
    free(prime_list);

    /* DP array: dp[k * stride + s] = number of ways to pick k distinct
     * primes that sum to s.  We need k from 0..8 and sums 0..n.  */
    size_t stride = (size_t)n + 1;          /* sums 0..n inclusive */
    seq_t *dp = (seq_t*)malloc(sizeof(seq_t) * 9 * stride);
    if (!dp) {
        free(primes);
        return 0;
    }
    /* Initialize all entries to zero.  Use memset because seq_t is
     * trivial. */
    memset(dp, 0, sizeof(seq_t) * 9 * stride);
    dp[0] = 1;                               /* 0 primes can sum to 0 */

    /* Iterate over each prime (including 1). */
    for (size_t pi = 0; pi < total_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p > n) {
            break;                           /* remaining primes are larger */
        }
        size_t max_s = (size_t)n - p;        /* highest sum index reachable */

        /* Update DP from higher k to lower to avoid reuse of same prime. */
        for (int k = 7; k >= 0; --k) {       /* k + 1 ≤ 8 */
            seq_t *rowK  = dp + (size_t)k * stride;
            seq_t *rowK1 = dp + (size_t)(k + 1) * stride;
            for (size_t s = 0; s <= max_s; ++s) {
                seq_t val = rowK[s];
                if (val) {
                    rowK1[s + p] += val;
                }
            }
        }
    }

    seq_t result = dp[8 * stride + n];

    free(dp);
    free(primes);
    return result;
}
