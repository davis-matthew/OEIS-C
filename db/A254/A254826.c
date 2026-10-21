/*
 * A254826  Number of length n 1..(7+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A254826 – Number of length n arrays of values 1..8 such that
 * every leading partial sum is divisible by 2, 3, or 5.
 * Computation: dynamic programming over the sum modulo 30 (lcm(2,3,5)).
 */
#include <stddef.h>   /* for size_t if needed */

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* State arrays on the stack – size 30 is negligible. */
    seq_t dp[30] = {0};
    seq_t ndp[30] = {0};
    dp[0] = (seq_t)1;   /* empty prefix sum */

    /* Main DP loop */
    for (idx_t step = 0; step < n; ++step) {
        /* clear ndp */
        for (unsigned int i = 0; i < 30; ++i) ndp[i] = (seq_t)0;

        for (unsigned int rem = 0; rem < 30; ++rem) {
            seq_t cur = dp[rem];
            if (cur == (seq_t)0) continue;

            for (unsigned int val = 1; val <= 8; ++val) {
                unsigned int newrem = rem + val;
                if (newrem >= 30) newrem -= 30;

                /* keep only prefixes whose sum is divisible by 2, 3, or 5 */
                if ((newrem % 2 == 0) || (newrem % 3 == 0) || (newrem % 5 == 0)) {
                    ndp[newrem] += cur;
                }
            }
        }

        /* swap dp and ndp for next step */
        for (unsigned int i = 0; i < 30; ++i) dp[i] = ndp[i];
    }

    /* Sum all reachable states – all are valid after the last step */
    seq_t ans = (seq_t)0;
    for (unsigned int i = 0; i < 30; ++i) ans += dp[i];

    return ans;
}
