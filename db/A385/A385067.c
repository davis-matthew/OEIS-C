/*
 * A385067  G.f.: Sum_{k>=0} x^k * Product_{j=1..3*k} (1 + x^j).
 * https://oeis.org/A385067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=0} number of subsets of {1..3k} that sum to (n-k)
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 206, u64 n <= 723,
 *            u128 n <= 2669, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate an array of n+1 counts.  The helper initializes each
       element to zero and constructs the seq_t objects correctly
       (works for native and arbitrary‑precision builds). */
    seq_t *dp = alloc_seq_array(n + 1);
    if (dp == NULL) abort();

    dp[0] = 1;                    /* empty subset contributes 1 */

    seq_t result = 0;
    result += dp[n];              /* k = 0 contributes dp[n] */

    /* current maximum element that has been added to the
       set of possible summands */
    idx_t current_max = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* New elements for this k are j = 3*(k-1)+1 .. 3*k */
        idx_t start = 3 * (k - 1) + 1;
        idx_t end   = 3 * k;

        /* Update dp for each new element j */
        for (idx_t j = start; j <= end; ++j) {
            for (idx_t s = n; s >= j; --s) {
                dp[s] += dp[s - j];
            }
        }
        current_max = end;

        /* Add contribution for this k: need subsets summing to n-k */
        result += dp[n - k];
    }

    free(dp);          /* free the array obtained from alloc_seq_array */
    return result;
}
