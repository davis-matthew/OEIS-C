/*
 * A308872  Sum of the second largest parts in the partitions of n into 6
 *          parts.
 * https://oeis.org/A308872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..floor(n/6)} Sum_{l=m..floor((n-m)/5)}
 *            Sum_{k=l..floor((n-l-m)/4)} Sum_{j=k..floor((n-k-l-m)/3)}
 *            Sum_{i=j..floor((n-j-k-l-m)/2)} i a(n) =
 *            sum_{t=1}^{floor((n-4)/2)} t * (# partitions of n into 6 parts
 *            with second part = t)
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* dp[k][s] holds number of partitions of s into exactly k parts all <= current t.
   We only need k=0..4 because the remaining 4 parts after fixing second part are counted.
   All counts fit in uint64_t for n <= 10000. */
seq_t a(idx_t n)
{
    if (n < 6)
        return 0;

    idx_t N = n;
    size_t size = (size_t)(N + 1);

    /* Allocate DP arrays */
    uint64_t *dp[5];
    for (int k = 0; k <= 4; ++k) {
        dp[k] = (uint64_t *)calloc(size, sizeof(uint64_t));
    }
    dp[0][0] = 1;          /* base: one way to partition 0 with 0 parts */

    /* Temporary array for prefix sums of dp[4] */
    uint64_t *sumdp4 = (uint64_t *)calloc(size, sizeof(uint64_t));

    seq_t result = 0;

    /* The second part cannot exceed (N-4)/2, otherwise at least 4 other parts of size 1 cannot fit */
    idx_t tmax = N / 2;
    for (idx_t t = 1; t <= tmax; ++t) {
        /* Update DP for current maximum part size t */
        for (int k = 1; k <= 4; ++k) {
            for (idx_t s = t; s <= N; ++s) {
                dp[k][s] += dp[k - 1][s - t];
            }
        }

        /* Compute prefix sums of dp[4] to answer range sum queries quickly */
        uint64_t acc = 0;
        for (size_t s = 0; s <= N; ++s) {
            acc += dp[4][s];
            sumdp4[s] = acc;
        }

        /* Count partitions where the second part equals t */
        idx_t max_m = N - 2 * t;                 /* maximum possible sum of the remaining 4 parts */
        uint64_t count_second = 0;
        if (max_m >= 4) {
            /* sum of dp[4][4 .. max_m] = prefix[max_m] - prefix[3] */
            count_second = sumdp4[max_m] - sumdp4[3];
        }

        /* Accumulate contribution of this t */
        result += (seq_t)t * (seq_t)count_second;
    }

    /* Free allocated memory */
    for (int k = 0; k <= 4; ++k)
        free(dp[k]);
    free(sumdp4);

    return result;
}
