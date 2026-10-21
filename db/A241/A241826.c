/*
 * A241826  Number of partitions p = [x(1), ..., x(k)], where x(1) >= x(2) >=
 *          ... >= x(k), of n such that min(x(i) - x(i-1)) >= number of
 *          distinct parts of p.
 * https://oeis.org/A241826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (if n=0) + Σ_{d≥1, d + d^2(d-1)/2 ≤ n} P_at_most(d, n -
 *            d - d^2(d-1)/2) where P_at_most(d, m) is the number of
 *            partitions of m into at most d parts. The DP for P_at_most uses
 *            the identity: P(k,n) = P(k-1,n) + P(k,n-k) (n ≥ k) with
 *            P(0,0)=1, P(0,n>0)=0. This counts partitions into parts ≤...
 * Range:     u8 n <= 47, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp_cache = NULL;    /* 1‑D array: dp_cache[k*(N+1)+n] */
static idx_t  cache_n = 0;        /* maximum n stored in dp_cache */
static idx_t  cache_d = 0;        /* maximum d stored in dp_cache */

/* Ensure dp_cache has dimensions (cache_d+1) × (cache_n+1),
   holding partitions into at most k parts for 0 ≤ k ≤ cache_d and 0 ≤ n ≤ cache_n.  */
static void ensure_dp(idx_t n, idx_t dmax)
{
    if (n > cache_n || dmax > cache_d) {
        idx_t old_n = cache_n;
        idx_t old_d = cache_d;

        /* free old table if any */
        if (dp_cache) free(dp_cache);

        cache_n = n;
        cache_d = dmax;

        size_t rows = (size_t)(cache_d + 1);
        size_t cols = (size_t)(cache_n + 1);
        size_t sz = rows * cols;
        dp_cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!dp_cache) return;   /* allocation failed – program will fail on use */

        /* row 0: partitions into at most 0 parts – only 1 for 0, 0 otherwise */
        for (size_t i = 0; i < cols; ++i)
            dp_cache[i] = (i == 0 ? (seq_t)1 : (seq_t)0);

        /* compute other rows */
        for (idx_t k = 1; k <= cache_d; ++k) {
            dp_cache[(size_t)k * cols + 0] = (seq_t)1;  /* P(k,0)=1 */
            for (idx_t s = 1; s <= cache_n; ++s) {
                seq_t val = dp_cache[(size_t)(k - 1) * cols + s];  /* P(k-1,s) */
                if (s >= k)
                    val += dp_cache[(size_t)k * cols + (s - k)];  /* P(k,s-k) */
                dp_cache[(size_t)k * cols + s] = val;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Determine maximal possible number of distinct parts (dmax). */
    idx_t dmax = 0;
    for (idx_t d = 1; ; ++d) {
        /* base = d + d^2(d-1)/2  (minimal sum of a partition with d parts) */
        idx_t base = d + (d * d * (d - 1)) / 2;
        if (base > n) break;
        dmax = d;
    }

    ensure_dp(n, dmax);

    seq_t total = (seq_t)0;
    for (idx_t d = 1; d <= dmax; ++d) {
        idx_t base = d + (d * d * (d - 1)) / 2;
        idx_t S = n - base;                 /* remaining sum after forcing the gaps */
        /* dp[d][S] is stored in dp_cache[d*(n+1)+S] */
        seq_t add = dp_cache[(size_t)d * (cache_n + 1) + S];
        total += add;
    }

    return total;
}
