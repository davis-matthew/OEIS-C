/*
 * A268452  Number of length-n 0..3 arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A268452: Number of length-n 0..3 arrays with no 
 * adjacent pair x,x+1 followed at any distance by x+1,x.
 * We compute it by dynamic programming over an automaton that
 * remembers which ascending pairs (0-1, 1-2, 2-3) have occurred.
 * State = (mask, last_value) where mask is a bitmask of seen ascending pairs.
 * Transition rules:
 *   - If last + 1 == next: set corresponding bit in mask.
 *   - If last - 1 == next: skip transition if the corresponding bit is already set.
 *   - Otherwise no restrictions.
 * The count for length n is the sum over all states after processing n positions.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* 0 entries – unused but harmless */

    /* dp[mask][last] – number of arrays ending with last value and state mask */
    seq_t dp[8][4];
    seq_t ndp[8][4];

    /* initialise: first element can be 0..3, no mask bits set */
    for (int m = 0; m < 8; ++m)
        for (int v = 0; v < 4; ++v)
            dp[m][v] = 0;
    for (int v = 0; v < 4; ++v)
        dp[0][v] = 1;

    for (idx_t pos = 1; pos < n; ++pos)
    {
        /* zero ndp */
        for (int m = 0; m < 8; ++m)
            for (int v = 0; v < 4; ++v)
                ndp[m][v] = 0;

        for (int m = 0; m < 8; ++m)
        {
            for (int last = 0; last < 4; ++last)
            {
                seq_t cur = dp[m][last];
                if (cur == 0) continue;

                for (int nxt = 0; nxt < 4; ++nxt)
                {
                    if (last + 1 == nxt)               /* ascending pair */
                    {
                        int bit = last;                /* 0,1,2 */
                        int newm = m | (1 << bit);
                        ndp[newm][nxt] += cur;
                    }
                    else if (last - 1 == nxt)          /* descending pair */
                    {
                        int bit = nxt;                 /* x such that x+1 = last */
                        if (m & (1 << bit))            /* forbidden */
                            continue;
                        ndp[m][nxt] += cur;
                    }
                    else                                /* no restriction */
                    {
                        ndp[m][nxt] += cur;
                    }
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int m = 0; m < 8; ++m)
            for (int v = 0; v < 4; ++v)
                dp[m][v] = ndp[m][v];
    }

    /* sum all states to get total number of arrays */
    seq_t total = 0;
    for (int m = 0; m < 8; ++m)
        for (int v = 0; v < 4; ++v)
            total += dp[m][v];

    return total;
}
