/*
 * A261610  Expansion of Product_{k>=0} (1 + x^(3*k+1))/(1 - x^(3*k+1)).
 * https://oeis.org/A261610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 + x^(3k+1)) / (1 -
 *            x^(3k+1))
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code uses only integer arithmetic.  It allocates two
   temporary arrays of seq_t objects, fills them according to the
   recurrence derived from the generating function, and returns the
   requested coefficient.  The algorithm runs in O(n²/3) time, which
   is well under the 2‑second limit for n up to about 10⁴. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Allocate DP arrays of size n+1 */
    seq_t *dp   = seq_alloc_new_array((size_t)(n + 1));
    seq_t *tmp  = seq_alloc_new_array((size_t)(n + 1));

    /* Initialise dp[0] = 1, others 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Process part sizes 1,4,7,... up to n */
    for (idx_t m = 1; m <= n; m += 3)
    {
        /* Re‑compute array tmp from current dp */
        for (idx_t r = 0; r < m; ++r)
        {
            seq_t cum = 0;
            for (idx_t j = r; j <= n; j += m)
            {
                /* tmp[j] = dp[j] + 2 * cum  (cumulative sum of previous terms) */
                tmp[j] = dp[j] + cum + cum;
                cum   += dp[j];
            }
        }
        /* Swap dp and tmp for the next iteration */
        seq_t *swap = dp;
        dp          = tmp;
        tmp         = swap;
    }

    /* Retrieve result and free temporary arrays */
    seq_t result = dp[n];
    seq_free_array(dp);
    seq_free_array(tmp);
    return result;
}
