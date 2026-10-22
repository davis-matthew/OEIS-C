/*
 * A375978  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 3 or 5.
 * https://oeis.org/A375978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Number of subsets of {1,2,...,n} with no two elements differing
 *            by 3 or 5. Computed by dynamic programming over the last 5
 *            positions: a DP state of 32 bit masks (5 bits). Complexity O(n)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* DP state: dp[mask] = number of ways to form a valid subset
       for the numbers processed so far, with 'mask' describing
       whether each of the last five positions was chosen.
       Bit 0 = position i-1 was chosen, bit 1 = i-2, ..., bit 4 = i-5. */
    seq_t dp[32] = {0};
    seq_t next[32] = {0};

    dp[0] = 1;                     /* empty set */
    for (idx_t i = 1; i <= n; ++i) {
        /* reset next state */
        for (int m = 0; m < 32; ++m) next[m] = 0;

        for (int mask = 0; mask < 32; ++mask) {
            seq_t val = dp[mask];
            if (val == 0) continue;

            /* 1. do not select i */
            int m1 = (mask << 1) & 0x1F;   /* drop bit for i-5 */
            next[m1] = next[m1] + val;

            /* 2. select i if it is allowed:
                 cannot pick i if i-3 or i-5 were chosen.
                 i-3 corresponds to bit 2, i-5 to bit 4. */
            if ((mask & (1 << 2)) == 0 && (mask & (1 << 4)) == 0) {
                int m2 = ((mask << 1) | 1) & 0x1F;  /* set bit for i */
                next[m2] = next[m2] + val;
            }
        }
        /* move to next step */
        for (int m = 0; m < 32; ++m) dp[m] = next[m];
    }

    /* sum over all final states */
    seq_t total = 0;
    for (int m = 0; m < 32; ++m) total = total + dp[m];
    return total;
}
