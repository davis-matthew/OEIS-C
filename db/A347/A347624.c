/*
 * A347624  Number of partitions of n into at most 5 prime parts (counting 1
 *          as a prime).
 * https://oeis.org/A347624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A347624
    Formula: a(n) = number of partitions of n into at most 5 parts chosen from {1} ∪ primes.
    Algorithm: 1‑sieve for primes ≤ n, build the part set {1}∪primes, then compute a 2‑dimensional
    DP table dp[k][s] where k∈[0,5] counts partitions of s using at most k parts.
    Complexity: O(#parts · 5 · n)  (≈ 6·n·π(n)), fast for n≤10000.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- 1. Elementary sieve to collect primes ≤ n ---- */
    size_t sz = (size_t)(n + 1);
    uint8_t *prime_flag = (uint8_t*)malloc(sz);
    if (!prime_flag) return (seq_t)0;
    for (size_t i = 0; i < sz; ++i) prime_flag[i] = 1;
    if (sz > 0) prime_flag[0] = 0;
    if (sz > 1) prime_flag[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= n; ++i)
        if (prime_flag[(size_t)i])
            for (idx_t j = i * i; j <= n; j += i)
                prime_flag[(size_t)j] = 0;

    /* ---- 2. Count primes to know parts array size ---- */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flag[(size_t)i]) ++prime_cnt;

    /* ---- 3. Build parts array: 1 followed by all primes ≤ n ---- */
    size_t parts_cnt = prime_cnt + 1;
    idx_t *parts = (idx_t*)malloc(parts_cnt * sizeof(idx_t));
    if (!parts) { free(prime_flag); return (seq_t)0; }
    parts[0] = 1;
    size_t idx = 1;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flag[(size_t)i]) parts[idx++] = i;
    free(prime_flag);

    /* ---- 4. DP table: dp[k][s] for k=0..5, s=0..n ---- */
    size_t stride = (size_t)(n + 1);
    size_t total = 6 * stride;
    seq_t *dp = (seq_t*)malloc(total * sizeof(seq_t));
    if (!dp) { free(parts); return (seq_t)0; }
    for (size_t i = 0; i < total; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;   /* dp[0][0] = 1 */

    for (size_t pi = 0; pi < parts_cnt; ++pi) {
        idx_t v = parts[(size_t)pi];
        if (v > n) continue;
        for (idx_t s = v; s <= n; ++s) {
            size_t sidx = (size_t)s;
            for (size_t k = 1; k <= 5; ++k) {
                size_t idx1 = k * stride + sidx;
                size_t idx2 = (k - 1) * stride + (size_t)(s - v);
                dp[idx1] += dp[idx2];
            }
        }
    }

    /* ---- 5. Sum contributions of ≤5 parts ---- */
    seq_t ans = 0;
    size_t nidx = (size_t)n;
    for (size_t k = 0; k <= 5; ++k)
        ans += dp[k * stride + nidx];

    free(dp);
    free(parts);
    return ans;
}
