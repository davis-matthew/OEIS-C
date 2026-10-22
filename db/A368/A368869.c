/*
 * A368869  Number of partitions of n into prime parts not greater than
 *          sqrt(n).
 * https://oeis.org/A368869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   _floor_isqrt_u64, alloc_seq_array, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into prime parts not greater than sqrt(n). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n) {
    /* Compute floor(sqrt(n)). */
    idx_t limit = _floor_isqrt_u64(n);

    /* Build the list of primes ≤ limit. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Allocate dynamic programming table dp[0..n] of type seq_t. */
    size_t dp_len = (size_t)n + 1;
    seq_t *dp = alloc_seq_array(dp_len);
    dp[0] = (seq_t)1;                 /* one way to partition 0 */

    /* Unbounded knapsack DP using each prime as a coin. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        for (idx_t j = (idx_t)p; j <= n; ++j) {
            dp[j] = (seq_t)(dp[j] + dp[j - (idx_t)p]);
        }
    }

    /* The desired value is dp[n]. */
    seq_t result = dp[n];

    /* Clean up. */
    free(dp);           /* allocated by alloc_seq_array */
    free(primes);       /* allocated by build_primes_upto */

    return result;
}
