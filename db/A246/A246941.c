/*
 * A246941  Number of partitions of n into 9 sorts of parts.
 * https://oeis.org/A246941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{i≥1} (1-9x^i)^(-1) (unbounded coloured
 *            partitions with 9 colours for each part). Recurrence (computed
 *            by DP): for all j≥i, dp[j] += 9*dp[j-i]
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"  /* alloc_seq_array(size_t) */

seq_t a(idx_t n)
{
    /* static cache of values up to current maximum index   */
    static seq_t *dp   = NULL;
    static idx_t  max_n = 0;

    /* 0 is a special case – return the only known value  */
    if (n == 0) {
        if (dp == NULL) {
            /* Allocate a single entry to store a(0)=1  */
            dp   = alloc_seq_array(1);
            dp[0] = 1;          /* fits in any seq_t  */
            max_n = 0;
        }
        return dp[0];
    }

    /* Need to recompute up to new maximum index    */
    if (dp == NULL || n > max_n) {

        /* Allocate new array large enough for 0…n   */
        seq_t *newdp = alloc_seq_array((size_t)(n + 1));
        if (newdp == NULL) {
            /* In a real environment you might exit or set errno, but
               the function contract does not require error handling.   */
            return 0;
        }

        /* initialise all cells to zero and set base case a(0)=1   */
        for (idx_t k = 0; k <= n; ++k) {
            newdp[k] = 0;
        }
        newdp[0] = 1;

        /* Classic unbounded‑knapsack DP – loop over part size i  */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; ++j) {
                /* Add contributions of a new part of size i (9 colours) */
                newdp[j] += (seq_t)9 * newdp[j - i];
            }
        }

        /* Replace old cache – free only if malloc/free used    */
        if (dp != NULL) {
            free(dp);
        }
        dp = newdp;
        max_n = n;
    }

    /* Return the cached value for requested index */
    return dp[n];
}
