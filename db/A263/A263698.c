/*
 * A263698  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -3 to 3 places and every three consecutive
 *          elements having its maximum within 4 of its minimum.
 * https://oeis.org/A263698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Dynamic programming counting permutations of 0..n‑1 with
 *            displacement ≤3 and with every three consecutive elements
 *            satisfying (max – min) ≤ 4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 65536 states * 7 bits mask ≈ 16 384 entries */
    seq_t dp[2][128][8][8];
    /* zero all dp cells */
    for (int t = 0; t < 2; ++t)
        for (int m = 0; m < 128; ++m)
            for (int s1 = 0; s1 < 8; ++s1)
                for (int s2 = 0; s2 < 8; ++s2)
                    dp[t][m][s1][s2] = 0;

    if (n == 0)    /* not used – offset is 1 */
        return 0;

    /* initial mask for position 0:
       values 0 … min(3, n−1) are available          */
    int init_mask = 0;
    for (idx_t val = 0; val < n && val <= 3; ++val)
        init_mask |= 1 << (val + 3);

    dp[0][init_mask][7][7] = (seq_t)1;      /* 7 is sentinel for “none” */

    int cur = 0, nxt = 1;
    for (idx_t j = 0; j < n; ++j)
    {
        /* clear next layer */
        for (int m = 0; m < 128; ++m)
            for (int s1 = 0; s1 < 8; ++s1)
                for (int s2 = 0; s2 < 8; ++s2)
                    dp[nxt][m][s1][s2] = 0;

        /* will be set for the new value j+4 if it exists */
        int new_bit = (j + 4 < n) ? 1 : 0;

        for (int mask = 0; mask < 128; ++mask)
            for (int s1 = 0; s1 < 8; ++s1)
                for (int s2 = 0; s2 < 8; ++s2)
                {
                    seq_t cnt = dp[cur][mask][s1][s2];
                    if (!cnt) continue;

                    for (int r = -3; r <= 3; ++r)
                    {
                        int bit = r + 3;               /* 0 … 6 */
                        if (!(mask & (1 << bit))) continue;

                        /* check triple constraint if we have two predecessors */
                        if (j >= 2)
                        {
                            int v1 = (int)((j - 1) + (s1 - 3));
                            int v2 = (int)((j - 2) + (s2 - 3));
                            int v  = (int)(j + r);

                            int mn = v1;
                            if (v2 < mn) mn = v2;
                            if (v  < mn) mn = v;
                            int mx = v1;
                            if (v2 > mx) mx = v2;
                            if (v  > mx) mx = v;

                            if (mx - mn > 4) continue;
                        }

                        int new_s1 = bit;          /* offset of current value relative to j   */
                        int new_s2 = s1;           /* previous s1 becomes the new s2          */

                        int new_mask = (mask & (~(1 << bit))) >> 1;
                        if (new_bit) new_mask |= 1 << 6;
                        new_mask &= 127;           /* keep it to 7 bits                       */

                        dp[nxt][new_mask][new_s1][new_s2] += cnt;
                    }
                }

        cur ^= 1;   /* swap layers */
        nxt ^= 1;
    }

    /* sum all states – the mask bits are irrelevant at the end */
    seq_t total = 0;
    for (int m = 0; m < 128; ++m)
        for (int s1 = 0; s1 < 8; ++s1)
            for (int s2 = 0; s2 < 8; ++s2)
                total += dp[cur][m][s1][s2];

    return total;
}
