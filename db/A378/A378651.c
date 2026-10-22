/*
 * A378651  a(n) = 1 if the divisors of n can be partitioned in exactly two
 *          ways into two disjoint sets with the same sum, otherwise 0.
 * https://oeis.org/A378651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9527, u16 n <= 9527, u32 n <= 9527, u64 n <= 9527,
 *            u128 n <= 9527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A378651: a(n) = 1 if the divisors of n can be partitioned in exactly
 * two unordered ways into two disjoint sets with equal sum, otherwise 0.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The offset of the sequence starts at 1. */
    if (n < 1u) return 0;

    uint64_t N = (uint64_t)n;
    /* We will store all divisors of N in this array.  For values up to
       10 000 256 entries are more than enough (max divisor count for
       10 400 is 25).  The array is static to avoid repeated dynamic
       allocation. */
    uint64_t divs[256];
    size_t dcnt = 0;
    uint64_t sigma = 0;

    /* Enumerate divisors by trial division.  The loop condition
       `i <= N / i` avoids overflow even for very large N. */
    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            divs[dcnt++] = i;
            sigma += i;
            uint64_t j = N / i;
            if (j != i) {
                divs[dcnt++] = j;
                sigma += j;
            }
        }
    }

    /* If the sum of divisors is odd, no equal‑sum partition exists. */
    if (sigma & 1ULL) return 0;

    uint64_t target_ull = sigma >> 1;
    /* We need a DP array of size `target + 1`.  If the target does not
       fit into a size_t we cannot allocate the array, so we return 0. */
    if (target_ull > SIZE_MAX) return 0;
    size_t target = (size_t)target_ull;

    uint64_t *dp = (uint64_t *)malloc((target + 1) * sizeof(uint64_t));
    if (!dp) return 0;
    for (size_t i = 0; i <= target; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Saturation threshold: as soon as any DP entry reaches this value
       we know that the number of unordered partitions will be > 2. */
    const uint64_t THRESH = 10ULL;

    /* Dynamic programming: count the number of subsets of the divisor set
       that sum to each possible total.  We iterate over the divisor set
       and update the DP array from high to low indices. */
    for (size_t idx = 0; idx < dcnt; ++idx) {
        uint64_t dval = divs[idx];
        if (dval > target) continue;
        size_t dsize = (size_t)dval;
        for (size_t s = target; s >= dsize; --s) {
            uint64_t add = dp[s - dsize];
            if (add == 0) continue;
            uint64_t new_val = dp[s] + add;
            dp[s] = (new_val >= THRESH) ? THRESH : new_val;
        }
    }

    uint64_t subset_cnt = dp[target];
    free(dp);

    /* Each unordered partition is counted twice (once for each complement). */
    uint64_t partitions = subset_cnt / 2;
    return (partitions == 2u) ? (seq_t)1 : (seq_t)0;
}
