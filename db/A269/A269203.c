/*
 * A269203  Number of 3 X n 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three no more than once.
 * https://oeis.org/A269203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A269203: Count of 3×n arrays over {0,1,2,3} with at most one
 * horizontally, diagonally or anti‑diagonally adjacent pair summing to 3.
 * Computed by dynamic programming over columns with at most 1 “bad” edge.
 * State: column value (3 cells, 6 bits ⇒ 64 states).
 * Transition cost (0..7) is number of adjacent pairs summing to 3 between two
 * consecutive columns.  Only transitions with total cost ≤1 are allowed.
 * Complexity: O(n·64²) ≈ 8 M operations for n=10 000.  Works for any
 * seq_t/idx_t combination because all arithmetic uses seq_t and tiny
 * integer constants.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return for the offset n=1 directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)64;   /* 4⁶ = 64 */
    
    /* Precompute transition costs between all 64 column states. */
    static unsigned char cost[64][64];
    static int cost_computed = 0;
    if (!cost_computed) {
        for (int s = 0; s < 64; ++s) {
            int a0 = (s >> 0) & 3;
            int a1 = (s >> 2) & 3;
            int a2 = (s >> 4) & 3;
            for (int t = 0; t < 64; ++t) {
                int b0 = (t >> 0) & 3;
                int b1 = (t >> 2) & 3;
                int b2 = (t >> 4) & 3;
                unsigned char c = 0;
                /* horizontal edges */
                if (a0 + b0 == 3) ++c;
                if (a1 + b1 == 3) ++c;
                if (a2 + b2 == 3) ++c;
                /* diagonal edges */
                if (a0 + b1 == 3) ++c;
                if (a1 + b2 == 3) ++c;
                /* anti‑diagonal edges */
                if (a1 + b0 == 3) ++c;
                if (a2 + b1 == 3) ++c;
                cost[s][t] = c;
            }
        }
        cost_computed = 1;
    }

    /* DP arrays: dp[state][k] = number of ways for current prefix
     * ending in 'state' with exactly k bad edges (k=0 or 1).  */
    seq_t dp[64][2] = {{0}};
    seq_t nextdp[64][2];

    for (int s = 0; s < 64; ++s) dp[s][0] = (seq_t)1;  /* first column */

    for (idx_t col = 2; col <= n; ++col) {
        /* Clear nextdp */
        for (int i = 0; i < 64; ++i) {
            nextdp[i][0] = (seq_t)0;
            nextdp[i][1] = (seq_t)0;
        }
        for (int s = 0; s < 64; ++s) {
            seq_t v0 = dp[s][0];
            seq_t v1 = dp[s][1];
            if (v0 == 0 && v1 == 0) continue;
            for (int t = 0; t < 64; ++t) {
                unsigned char c = cost[s][t];
                /* Transition from state s (k=0) */
                if (v0 != 0 && c <= 1) {
                    if (c == 0)
                        nextdp[t][0] += v0;
                    else
                        nextdp[t][1] += v0;
                }
                /* Transition from state s (k=1) – only allowed if no new bad edges */
                if (v1 != 0 && c == 0)
                    nextdp[t][1] += v1;
            }
        }
        /* Copy back to dp for next iteration */
        for (int i = 0; i < 64; ++i) {
            dp[i][0] = nextdp[i][0];
            dp[i][1] = nextdp[i][1];
        }
    }

    /* Sum all entries of the last DP stage */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 64; ++s) {
        result += dp[s][0];
        result += dp[s][1];
    }
    return result;
}
