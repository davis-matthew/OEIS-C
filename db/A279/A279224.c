/*
 * A279224  Expansion of Product_{k>=1} 1/(1 - x^(k*(k+1)*(2*k-1)/2)).
 * https://oeis.org/A279224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculate a(n) for OEIS sequence A279224
 * a(n) = number of partitions of n into octagonal pyramidal numbers
 *     = number of partitions of n using parts k*(k+1)*(2*k-1)/2 for k>=1
 * The implementation uses simple unbounded‑coin DP, recomputing for
 * each call.  Complexity is O(n * sqrt(n)), which is easily below the
 * time limit for n up to about 10 000.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure */

    /* initialise to zero; dp[0] will be 1 later */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* iterate over all octagonal pyramidal numbers <= n */
    unsigned long long k = 1;
    for (;; ++k) {
        unsigned long long part_ll =
            k * (k + 1) * (2 * k - 1) / 2;      /* k*(k+1)*(2k-1)/2 */

        if (part_ll > (unsigned long long)n) break;

        idx_t part = (idx_t)part_ll;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
