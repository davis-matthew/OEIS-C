/*
 * A242216  Number of partitions of n into Heegner numbers, cf. A003173.
 * https://oeis.org/A242216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into the set
 *            {1,2,3,7,11,19,43,67,163}. This is the standard unbounded‑coin
 *            change dynamic‑programming count
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* parts that are allowed in the partition */
    static const idx_t parts[9] = {1, 2, 3, 7, 11, 19, 43, 67, 163};
    /* allocate array for dp[0..n] */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (dp == NULL) return 0; /* out of memory – return 0 */

    /* initialise dp[0] = 1, others 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* standard unbounded‑coin DP */
    for (size_t p = 0; p < 9; ++p) {
        idx_t coin = parts[p];
        for (idx_t sum = coin; sum <= n; ++sum) {
            dp[sum] += dp[sum - coin];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
