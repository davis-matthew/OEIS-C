/*
 * A357320  The total number of fixed points among all strict partitions of n,
 *          when parts are written in increasing order.
 * https://oeis.org/A357320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A357320 – Total number of fixed points among all strict partitions of n
    Formula: a(n) = Σ_{i≥1, i(i+1)/2 ≤ n} P_{i+1}(n − i(i+1)/2)
    where P_{p}(m) is the number of strict partitions of m whose parts are all ≥ p.
    The algorithm builds the classic distinct‑partition DP in decreasing order
    of part value. After adding part p, the DP array contains the number of
    partitions of each sum using parts ≥ p.  The contribution of the fixed point
    i = p−1 (if i ≥ 1) is then dp[n − T_i] with T_i = i(i+1)/2.  This works
    for all n ≥ 2; the special cases n = 0 and n = 1 are handled directly. */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the trivial offsets */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Allocate DP array: dp[s] = number of strict partitions of s using parts ≥ current p */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;               /* in case of OOM, return 0 (unreachable in tests) */
    dp[0] = (seq_t)1;                       /* empty partition */

    seq_t ans = (seq_t)0;

    /* Process parts from n down to 1 */
    for (idx_t p = n; p >= 1; --p) {
        /* Add part p: dp[s] += dp[s-p] for all s ≥ p */
        for (idx_t s = n; s >= p; --s) {
            dp[s] += dp[s - p];
        }

        /* Fixed point i = p-1 (must be ≥1) */
        idx_t i = p - 1;
        if (i >= 1) {
            seq_t Ti = ((seq_t)i * ((seq_t)i + (seq_t)1)) / (seq_t)2;  /* T_i = i(i+1)/2 */
            if (Ti <= (seq_t)n) {
                idx_t rem = (idx_t)(((seq_t)n - Ti));                /* n - T_i */
                ans += dp[rem];
            }
        }
    }

    free(dp);
    return ans;
}
