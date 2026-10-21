/*
 * A272496  Number of ordered set partitions of [n] with nondecreasing block
 *          sizes and maximal block size equal to six.
 * https://oeis.org/A272496
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A272496:
   Let dp[n][k] be the sum of n! / Π block! over all ordered
   partitions of [n] into blocks of sizes ≤6, with the last block size
   equal to k (1≤k≤6). Then
       dp[n][k] = 0                       if n < k
       dp[k][k] = 1                       // single block of size k
       dp[n][k] = C(n, k) * Σ_{t=1..k} dp[n-k][t]   if n > k
   The desired value is a(n) = dp[n][6] (for n≥6).  */

/* Ensure the dynamic programming table is ready for all indices ≤ n. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b) */

static seq_t *dp = NULL;      /* flat array: dp[n*7 + k], k = 1..6 */
static idx_t dp_max = 0;      /* largest n computed (dp_max = 0 means none) */

static void ensure_dp(idx_t n)
{
    if (dp_max > n) return;

    size_t new_size = ((size_t)n + 1) * 7;
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    dp = tmp;

    for (idx_t cur = dp_max; cur <= n; ++cur) {
        for (idx_t last = 1; last <= 6; ++last) {
            if (cur < last) {
                dp[cur * 7 + last] = 0;
            } else if (cur == last) {
                dp[cur * 7 + last] = 1;
            } else {
                seq_t bin = binom(cur, last);
                seq_t sum = 0;
                for (idx_t prev = 1; prev <= last; ++prev) {
                    seq_t val = dp[(cur - last) * 7 + prev];
                    if (val) sum += val * bin;
                }
                dp[cur * 7 + last] = sum;
            }
        }
    }
    dp_max = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* undefined for n < 6 */
    ensure_dp(n);
    return dp[n * 7 + 6];          /* last block size must be 6 */
}
