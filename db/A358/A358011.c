/*
 * A358011  Number of partitions of n into at most 6 distinct prime parts.
 * https://oeis.org/A358011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into at most 6 distinct prime
 *            parts. This is computed by a 0‑1 knapsack DP over the primes ≤ n
 * Range:     u8 n <= 110, u16 n <= 467, u32 n <= 6845, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Count partitions of n into at most 6 distinct primes. */
seq_t a(idx_t n)
{
    /* Special‐case n = 0:  the empty sum is one partition. */
    if (n == 0) return (seq_t)1;

    /* ---------- 1. Sieve primes up to n ---------- */
    bool *isPrime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!isPrime) return (seq_t)0;          /* allocation failure */

    for (idx_t i = 0; i <= n; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= n; j += i) isPrime[j] = false;
        }
    }

    /* Collect primes into an array.  n is small enough to use a fixed size array. */
    uint64_t *primes = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!primes) {
        free(isPrime);
        return (seq_t)0;
    }
    idx_t primeCnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (isPrime[i]) primes[primeCnt++] = i;
    free(isPrime);

    /* ---------- 2. DP array: dp[s*7 + c] = #ways to make sum s using exactly c primes. */
    size_t dpSize = (size_t)(n + 1) * 7;
    seq_t *dp = (seq_t *)malloc(dpSize * sizeof(seq_t));
    if (!dp) {
        free(primes);
        return (seq_t)0;
    }

    /* initialize all entries to 0 */
    for (size_t i = 0; i < dpSize; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;   /* dp[0][0] = 1 */

    for (idx_t pi = 0; pi < primeCnt; ++pi) {
        uint64_t p = primes[pi];
        for (idx_t s = n; s >= p; --s) {
            size_t baseIdx = s * 7;
            size_t prevIdx = (s - p) * 7;
            /* update counts for c from 6 down to 1 */
            for (idx_t c = 6; c >= 1; --c) {
                dp[baseIdx + c] += dp[prevIdx + (c - 1)];
            }
        }
    }
    free(primes);

    /* ---------- 3. Sum over all allowed counts (0..6) ---------- */
    seq_t ans = (seq_t)0;
    for (idx_t c = 0; c <= 6; ++c) {
        ans += dp[n * 7 + c];
    }

    free(dp);
    return ans;
}
