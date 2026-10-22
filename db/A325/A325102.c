/*
 * A325102  Number of ordered pairs of positive integers up to n with no
 *          binary carries.
 * https://oeis.org/A325102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs of positive integers up to n with
 *            no binary carries
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count ordered pairs (i,j), 1≤i,j≤n, such that i & j == 0.  The implementation uses a digit‑DP
   over the binary representation of n.  Count all pairs with i,j in [0,n], then subtract
   the 2·(n+1)−1 pairs that involve a zero, leaving only the positive pairs.  Complexity
   O(⌊log₂n⌋·4·4) per query, well within the limits for n≤10⁴. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Zero and negative indices are outside the domain; return 0 for n = 0. */
    if (n == 0)
        return 0;

    /* Find the position of the most significant set bit. */
    idx_t msb = 63;
    while ((n >> msb) == 0)
        msb--;

    /* DP arrays: dp[ti][tj] counts partial assignments for i/j with
       tight flags ti,tj (1 = prefix equals n so far). */
    seq_t dp[2][2] = {{0}}, ndp[2][2] = {{0}};
    dp[1][1] = 1;  /* start with empty prefixes, both tight */

    for (idx_t pos = msb; pos != (idx_t)-1; --pos) {
        int nbit = (n >> pos) & 1;
        /* reset ndp */
        ndp[0][0] = ndp[0][1] = ndp[1][0] = ndp[1][1] = 0;

        for (int ti = 0; ti <= 1; ++ti) {
            for (int tj = 0; tj <= 1; ++tj) {
                seq_t cur = dp[ti][tj];
                if (!cur) continue;

                /* Allowed bits for i and j depend on tightness. */
                int maxb_i = ti ? nbit : 1;
                int maxb_j = tj ? nbit : 1;
                for (int bi = 0; bi <= maxb_i; ++bi) {
                    for (int bj = 0; bj <= maxb_j; ++bj) {
                        if (bi & bj) continue;  /* carry would occur */

                        int nti = ti && (bi == nbit);
                        int ntj = tj && (bj == nbit);
                        ndp[nti][ntj] += cur;
                    }
                }
            }
        }
        /* copy ndp to dp for next iteration */
        dp[0][0] = ndp[0][0]; dp[0][1] = ndp[0][1];
        dp[1][0] = ndp[1][0]; dp[1][1] = ndp[1][1];
    }

    /* Sum all reachable tight states: all i,j ≤ n satisfying no carry. */
    seq_t total_all = 0;
    for (int ti = 0; ti <= 1; ++ti)
        for (int tj = 0; tj <= 1; ++tj)
            total_all += dp[ti][tj];

    /* Subtract pairs involving a zero (i==0 or j==0). 
       Count of such pairs is 2·(n+1) – 1. */
    seq_t sub = ((seq_t)n << 1) + 1;
    return total_all - sub;
}
