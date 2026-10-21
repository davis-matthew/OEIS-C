/*
 * A266781  Molien series for invariants of finite Coxeter group A_12.
 * https://oeis.org/A266781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts of size 2..13
 * Range:     u8 n <= 23, u16 n <= 62, u32 n <= 242, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* g.f. = 1 / Prod_{k=2}^{13} (1 - t^k) */

#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Return the n-th term of OEIS sequence A266781. */
seq_t a(idx_t n)
{
    /* Allocate array dp[0..n] and initialise to zero. */
    seq_t *dp = alloc_seq_array((size_t)n + 1);
    if (!dp)
        return 0;  /* out of memory → 0 */

    dp[0] = 1;   /* a(0) = 1 */

    /* Classic coin‑change DP: count partitions with parts 2..13. */
    for (idx_t coin = 2; coin <= 13; ++coin) {
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
