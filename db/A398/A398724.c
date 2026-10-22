/*
 * A398724  Number of partitions of n into parts congruent to 2, 3 or 7 (mod
 *          8).
 * https://oeis.org/A398724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts ≡ 2, 3, or 7 (mod 8)
 * Range:     u8 n <= 43, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: standard coin‑change DP treating the allowed part sizes as coins. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate DP array (handled by the helper to avoid C++ constructor issues). */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (dp == NULL) return 0;        /* allocation failed – return zero as a safe default */

    dp[0] = (seq_t)1;                /* one way to partition 0 */

    /* Iterate over all allowed part sizes up to n. */
    for (idx_t d = 2; d <= n; ++d) {
        idx_t r = d & 7;             /* d % 8 using bitwise & 7 for speed */
        if (r == 2 || r == 3 || r == 7) {
            for (idx_t i = d; i <= n; ++i) {
                dp[i] += dp[i - d];
            }
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
