/*
 * A256012  Number of partitions of n into distinct parts that are not
 *          squarefree.
 * https://oeis.org/A256012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts that are
 *            NOT * squarefree. Equivalently, a(n) is the number of subsets of
 *            the set * S = {k ≥ 1 | k is not squarefree} that sum to n. A
 *            simple dynamic * programming solution runs in O(n²) time and
 *            uses O(n) space, * which is adequate for n up to...
 * Range:     u8 n <= 100, u16 n <= 243, u32 n <= 679, u64 n <= 2150,
 *            u128 n <= 7441, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1.  The array is indexed by normal
     * integer indices so we use idx_t for loop counters. */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (dp == NULL) return 0;              /* Sanity check */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                              /* Empty subset sums to 0 */

    /* Iterate over all possible parts k (1 … n).  Only non-squarefree
     * numbers contribute because squarefree parts are forbidden. */
    for (idx_t k = 1; k <= n; ++k) {
        if (is_squarefree((seq_t)k)) continue;   /* Skip squarefree k */

        /* Standard 0/1 knapsack DP: process in reverse order so that
         * each part can be used at most once. */
        for (idx_t j = n; j >= k; --j) {
            dp[j] += dp[j - k];
        }
    }

    seq_t ans = dp[n];
    free_seq_array(dp);
    return ans;
}
