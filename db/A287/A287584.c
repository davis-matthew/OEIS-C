/*
 * A287584  Number of set partitions of [n] such that all absolute differences
 *          between least elements of consecutive blocks and between
 *          consecutive elements within the blocks are not larger than five.
 * https://oeis.org/A287584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] such that * every
 *            consecutive difference within blocks and the difference *
 *            between the least elements of consecutive blocks is ≤ 5. * *
 *            This implementation uses a dynamic programming state that *
 *            tracks, for each processed prefix, the set of active blocks....
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;

    /* DP arrays */
    static seq_t curr[64][7];
    static seq_t next[64][7];

    /* initialise after processing the first element (i = 1) */
    for (int m = 0; m < 64; ++m)
        for (int d = 0; d < 7; ++d)
            curr[m][d] = (seq_t)0;
    curr[1][0] = (seq_t)1;   /* one active block, last element at distance 0,
                                and most recent start also at 0 */

    /* process elements 2 .. n */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* zero next array */
        for (int m = 0; m < 64; ++m)
            for (int d = 0; d < 7; ++d)
                next[m][d] = (seq_t)0;

        for (int mask = 0; mask < 64; ++mask)
        {
            for (int d = 0; d < 7; ++d)
            {
                seq_t ways = curr[mask][d];
                if (ways == (seq_t)0) continue;

                /* If there are no active blocks, we must start a new one */
                if (mask == 0)
                {
                    if (d <= 4)          /* allowed: difference d+1 ≤ 5 */
                    {
                        int mask2 = 1;   /* new block at distance 0 */
                        int d2    = 0;
                        next[mask2][d2] += ways;
                    }
                    continue;
                }

                /* 1. Add the element to an existing active block */
                for (int k = 0; k <= 4; ++k)
                {
                    if (!(mask & (1 << k))) continue;      /* block has dist k? */

                    int mask2 = 0;
                    /* other active blocks increase their distances */
                    for (int t = 0; t <= 5; ++t)
                    {
                        if (!(mask & (1 << t))) continue;
                        if (t == k) continue;       /* this block will be reset */
                        int nt = t + 1;
                        if (nt <= 5) mask2 |= (1 << nt);
                    }
                    mask2 |= 1 << 0;               /* the updated block at dist 0 */

                    int d2 = (d < 6) ? d + 1 : 6;
                    next[mask2][d2] += ways;
                }

                /* 2. Start a new block at i (if allowed) */
                if (d <= 4)                     /* d+1 ≤ 5 */
                {
                    int mask2 = 0;
                    for (int t = 0; t <= 5; ++t)
                    {
                        if (!(mask & (1 << t))) continue;
                        int nt = t + 1;
                        if (nt <= 5) mask2 |= (1 << nt);
                    }
                    mask2 |= 1 << 0;               /* new block at dist 0 */

                    int d2 = 0;
                    next[mask2][d2] += ways;
                }
            }
        }

        /* swap curr and next */
        for (int m = 0; m < 64; ++m)
            for (int d = 0; d < 7; ++d)
                curr[m][d] = next[m][d];
    }

    /* sum over all states to obtain a(n) */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < 64; ++mask)
        for (int d = 0; d < 7; ++d)
            result += curr[mask][d];

    return result;
}
