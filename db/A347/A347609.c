/*
 * A347609  Number of partitions of n into at most 5 prime parts.
 * https://oeis.org/A347609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A347609: number of partitions of n into at most 5 prime parts.
 * This is computed with a dynamic‑programming “coin‑change” style
 * algorithm that counts partitions using exactly k prime parts
 * (k = 1..5) and then sums the results.  The algorithm is O(n⋅π(n))
 * which is well below the time limits for n up to ~10⁴.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases directly. */
    if (n == (idx_t)0)              return (seq_t)1;
    if (n == (idx_t)1)              return (seq_t)0;

    /* Generate all primes ≤ n.  The helper returns them in a freshly
       allocated array that must be freed by the caller. */
    size_t prime_cnt = 0;
    idx_t *primes = NULL;
    build_primes_upto(n, &primes, &prime_cnt);
    if (!primes) return (seq_t)0;   /* allocation failure */

    /* DP table: dp[s*6 + k] holds the number of ways to form sum s
       with exactly k prime parts (k = 0…5). */
    size_t N = (size_t)n;
    size_t dp_size = (N + 1) * 6;
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) { free(primes); return (seq_t)0; }

    /* Base condition: one way to write 0 with 0 parts. */
    dp[0] = (seq_t)1;

    /* Standard coin‑change DP: iterate over each prime and update
       the table for all attainable sums. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t prime = primes[pi];
        if (prime > n) break;   /* primes are sorted ascending */
        size_t p = (size_t)prime;
        for (size_t s = p; s <= N; ++s) {
            size_t base_s   = s * 6;
            size_t base_pre = (s - p) * 6;
            for (int k = 1; k <= 5; ++k) {
                dp[base_s + k] += dp[base_pre + (k - 1)];
            }
        }
    }

    /* Sum all possibilities having 1–5 parts. */
    seq_t result = (seq_t)0;
    size_t baseN = N * 6;
    for (int k = 1; k <= 5; ++k) {
        result += dp[baseN + k];
    }

    free(dp);
    free(primes);
    return result;
}
