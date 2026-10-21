/*
 * A294531  E.g.f.: 1/Product_{k>0} (1-x^(2*k-1)/(2*k-1)!).
 * https://oeis.org/A294531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294531
 * Formula: a(n) = Sum_{m odd, 1<=m<=n} C(n,m) * a(n-m)
 * with a(0) = 1.
 * Sequence a(n) counts the number of partitions of n into odd parts with
 * weight n! / ∏(k!^{cnt_k}).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_computed = 0;

    /* allocate or enlarge array to hold values up to n */
    if (!dp || dp_computed < n) {
        /* recompute the whole sequence up to n from scratch */
        dp_computed = n;
        dp = (seq_t *)realloc(dp, (size_t)(dp_computed + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= dp_computed; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* unbounded knapsack DP over odd part sizes */
        for (idx_t m = 1; m <= dp_computed; m += 2) {
            seq_t binom = (seq_t)1;                     /* C(m,m) */
            for (idx_t s = m; s <= dp_computed; ++s) {
                dp[s] = dp[s] + dp[s - m] * binom;
                if (s < dp_computed) {
                    /* binom = binom * (s+1) / (s+1-m)  (exact division) */
                    binom = binom * (seq_t)(s + 1) / (seq_t)(s + 1 - m);
                }
            }
        }
    }

    return dp[n];
}
