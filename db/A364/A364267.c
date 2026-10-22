/*
 * A364267  Total number of blocks in all set partitions of [n] such that each
 *          element is contained in a block whose index parity coincides with
 *          the parity of the element.
 * https://oeis.org/A364267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 31, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based on block parity constraints:
   Let dp[i][k] be the number of valid partitions of [i] with exactly k blocks.
   Let parity(i) = i & 1.
   Define t_even(k) = k/2          // number of even indices ≤ k
           t_odd(k)  = (k+1)/2     // number of odd indices ≤ k
   Then for i ≥ 1:
       dp[i][k] = dp[i-1][k] * (parity(i) ? t_odd(k) : t_even(k))
                  + ( (k & 1) == parity(i) ? dp[i-1][k-1] : 0 );
   The required sequence is
       a(n) = Σ_{k=0..n} k * dp[n][k] .
*/

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    /* Handle the trivial case directly */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Allocate two rows of DP tables */
#ifdef __cplusplus
    seq_t *dp_prev = new seq_t[n + 1];
    seq_t *dp_curr = new seq_t[n + 1];
    /* initialise to zero */
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        dp_prev[i] = (seq_t)0;
        dp_curr[i] = (seq_t)0;
    }
    dp_prev[0] = (seq_t)1;   /* dp[0][0] = 1 */
#else
    seq_t *dp_prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp_prev || !dp_curr) return (seq_t)0;  /* out of memory */
    dp_prev[0] = (seq_t)1;   /* dp[0][0] = 1 */
#endif

    /* Main DP loop */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* Zero the current row */
#ifdef __cplusplus
        for (idx_t k = (idx_t)0; k <= i; ++k)
            dp_curr[k] = (seq_t)0;
#else
        for (idx_t k = (idx_t)0; k <= i; ++k)
            dp_curr[k] = (seq_t)0;
#endif

        int p = (int)(i & (idx_t)1);   /* parity of i: 0 even, 1 odd */

        /* Existing block contributions */
        for (idx_t k = (idx_t)0; k < i; ++k) {
            seq_t val = dp_prev[k];
            if (val == (seq_t)0) continue;

            idx_t t = (p == 1) ? ((k + (idx_t)1) >> 1)
                              : (k >> 1);   /* t_even/k or t_odd/(k+1) */

            if (t != (idx_t)0)
                dp_curr[k] += val * (seq_t)t;
        }

        /* New block contributions */
        for (idx_t k = (idx_t)1; k <= i; ++k) {
            if (((k & (idx_t)1) == (idx_t)p)) {
                dp_curr[k] += dp_prev[k - (idx_t)1];
            }
        }

        /* Swap rows for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Compute the weighted sum for a(n) */
    seq_t result = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        result += dp_prev[k] * (seq_t)k;
    }

    /* Clean up */
#ifdef __cplusplus
    delete [] dp_prev;
    delete [] dp_curr;
#else
    free(dp_prev);
    free(dp_curr);
#endif

    return result;
}
