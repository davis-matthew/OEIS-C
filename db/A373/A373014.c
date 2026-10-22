/*
 * A373014  Number of partitions p of n such that max(p) == 1 mod 3.
 * https://oeis.org/A373014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A373014
    Number of partitions p of n such that max(p) = 1 (mod 3)
    Generation by dynamic programming:
        dp[i]   = number of partitions of i with parts <= current m
        res[i]  = desired a(i)
    For each part size m (1 .. n) we do
        old = dp[i];
        dp[i] += dp[i-m];
        if (m % 3 == 1)   res[i] += dp[i] - old;
    Complexity: O(n^2) time, O(n) memory per re‑computation.
*/

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static state between calls */
    static idx_t current_max = (idx_t)-1; /* maximum n already computed */
    static seq_t *dp = NULL;             /* dp array   (size current_max + 1) */
    static seq_t *res = NULL;            /* res array  (size current_max + 1) */

    /* recompute all values up to n if we do not have them yet */
    if (current_max == (idx_t)-1 || n > current_max) {
        /* allocate arrays for the new range [0..n] */
        dp  = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        res = (seq_t *)realloc(res, (n + 1) * sizeof(seq_t));

        /* initialise */
        dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i) {
            dp[i] = (seq_t)0;
            res[i] = (seq_t)0;
        }

        /* dynamic programme */
        for (idx_t m = 1; m <= n; ++m) {
            for (idx_t i = m; i <= n; ++i) {
                seq_t old = dp[i];
                dp[i] += dp[i - m];
                if (m % 3 == 1) {
                    res[i] += dp[i] - old;
                }
            }
        }
        current_max = n;
    }

    return res[n];
}
