/*
 * A378248  Number of sets of chess pieces whose collective material value
 *          adds to n.
 * https://oeis.org/A378248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 148, u32 n <= 2531, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A378248
 *  Number of sets of chess pieces whose collective material value adds to n.
 *
 *  A piece can be valued 1 (pawn), 3a (knight), 3b (bishop), 5 (rook),
 *  or 9 (queen).  The two 3's are considered distinct, so partitions
 *  with different numbers of 3a and 3b are counted separately.
 *
 *  The sequence can be computed as the coefficient of
 *  1/((1-x)*(1-x^3)^2*(1-x^5)*(1-x^9)).
 *
 *  We use a simple unbounded–knapsack DP that runs in O(n) time:
 *
 *      dp[0] = 1
 *      for each coin value v in {1,3,3,5,9}:
 *          for i = v .. n:
 *              dp[i] += dp[i-v]
 *
 *  This treats the two 3's as distinct coins, giving the correct
 *  partition counts.
 *
 *  The function is safe to call in any order and for any n >= 0.
 *  It performs its own allocation of a temporary array of length n+1.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset 0 (empty set) */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate temporary DP array */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;  /* Out‑of‑memory: return 0 (unlikely in normal use) */

    /* Initialise DP table */
    dp[0] = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Coin values: pawn=1, 3a=3, 3b=3, rook=5, queen=9 */
    const idx_t coins[5] = {1, 3, 3, 5, 9};
    for (int c = 0; c < 5; ++c) {
        idx_t v = coins[c];
        for (idx_t i = v; i <= n; ++i)
            dp[i] = dp[i] + dp[i - v];
    }

    /* Result */
    seq_t result = dp[n];
    free(dp);
    return result;
}
