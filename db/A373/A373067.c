/*
 * A373067  Number of partitions of n such that (smallest part) >= 2*(number
 *          of parts).
 * https://oeis.org/A373067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For each k with 2*k*k ≤ n we subtract 2*k from each part. The
 *            number of resulting partitions equals the number of partitions
 *            of m = n-2*k*k into at most k parts. We compute those counts by
 *            a small DP on parts ≤ k and sum over all admissible k
 * Range:     u8 n <= 52, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* integer square‑root floor for 64‑bit values */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid != 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* empty partition */

    /* largest k satisfies 2*k*k ≤ n → k ≤ sqrt(n/2) */
    idx_t k_max = isqrt_u64(n / 2);

    size_t N = (size_t)n;
    size_t K = (size_t)k_max;
    size_t cols = N + 1;
    size_t rows = K + 1;

    seq_t *dp = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!dp) return (seq_t)0;              /* allocation failure */

    /* base row: k = 0 → only sum 0 has one partition */
    dp[0] = (seq_t)1;

    /* DP: dp[k][s] = dp[k-1][s] + dp[k][s-k]  if s ≥ k */
    for (idx_t k = 1; k <= k_max; ++k) {
        size_t base = (size_t)k * cols;
        size_t prev = (size_t)(k - 1) * cols;
        for (idx_t s = 0; s <= n; ++s) {
            size_t idx = base + (size_t)s;
            dp[idx] = dp[prev + (size_t)s];
            if (s >= k)
                dp[idx] += dp[base + ((size_t)s - (size_t)k)];
        }
    }

    /* Sum contributions for each admissible k */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= k_max; ++k) {
        idx_t rem = n - (2 * k * k);
        size_t idx = (size_t)k * cols + (size_t)rem;
        result += dp[idx];
    }

    free(dp);
    return result;
}
