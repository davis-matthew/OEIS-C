/*
 * A373934  Number of compositions of 7*n-2 into parts 6 and 7.
 * https://oeis.org/A373934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of 7*n-2 into parts 6 and 7.
   This implementation uses a simple dynamic‑programming recurrence:
   f(m) = f(m‑6) + f(m‑7) with f(0) = 1 (empty composition) and
   f(m) = 0 for 0 < m < 6.  The desired term is a(n) = f(7*n‑2).  */
/*  The sequence can be computed on demand and cached for fast
   repeated calls.  */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

/* External linkage as required by the framework. */
seq_t a(idx_t n)
{
    /* a(1) = 0, because 7*1-2 = 5 cannot be formed from 6 and 7. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Target sum m = 7*n - 2.  */
    idx_t m = 7 * n - 2;

    /* Static cache for the DP array.  We keep the maximum index
       currently computed in dp_len.  */
    static seq_t *dp      = NULL;     /* DP array of f(i) values.  */
    static size_t dp_cap = 0;         /* Current capacity of dp. */
    static idx_t  dp_len = 0;         /* Next index that is not yet valid. */

    /* Ensure the DP array is large enough to hold indices up to m.  */
    ensure_capacity(m + 1, &dp_cap, (void **)&dp, sizeof(seq_t));

    /* Initialise the base value f(0) = 1 if this is the first run.  */
    if (dp_len == 0) {
        dp[0] = (seq_t)1;
        dp_len = 1;
    }

    /* Compute all missing values up to index m.  */
    for (idx_t i = dp_len; i <= m; ++i) {
        seq_t val6 = (i >= 6) ? dp[i - 6] : (seq_t)0;
        seq_t val7 = (i >= 7) ? dp[i - 7] : (seq_t)0;
        dp[i] = val6 + val7;
    }

    /* Update the computed length.  */
    dp_len = m + 1;

    /* The required term.  */
    return dp[m];
}
