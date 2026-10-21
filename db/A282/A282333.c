/*
 * A282333  Number of nX3 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×3 binary arrays with exactly one 1 that has more
 *            than four king‑move neighbours. 24-bit state DP, one transition
 *            per row, O(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Pre‑computed number of violations (number of 1‑cells exceeding the
   neighbour limit) in the centre row 'cur', given the three
   adjacent rows 'prev', 'cur', 'next'. */
static uint8_t viol_table[8][8][8];
static int viol_table_inited = 0;

static void init_viol_table(void)
{
    if (viol_table_inited) return;
    viol_table_inited = 1;
    for (int pp = 0; pp < 8; ++pp)
        for (int cc = 0; cc < 8; ++cc)
            for (int nn = 0; nn < 8; ++nn)
            {
                int viol = 0;
                for (int col = 0; col < 3; ++col)
                {
                    if (((cc >> col) & 1) == 0) continue;   /* center cell is 0 */
                    int ones = 0;
                    for (int dr = -1; dr <= 1; ++dr)
                        for (int dc = -1; dc <= 1; ++dc)
                        {
                            if (dr == 0 && dc == 0) continue;
                            int rrow;
                            if (dr == -1) rrow = pp;
                            else if (dr == 0) rrow = cc;
                            else rrow = nn;
                            int ccol = col + dc;
                            if (ccol < 0 || ccol > 2) continue;
                            int bit = (rrow >> ccol) & 1;
                            ones += bit;
                        }
                    if (ones > 4) ++viol;
                }
                viol_table[pp][cc][nn] = (uint8_t)viol;
            }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* argument < offset, return 0 */
    init_viol_table();

    /* dp[prev][cur][viol] – viol 0 or 1 only, overflowed states are discarded */
    seq_t dp[8][8][2] = {0};
    seq_t newdp[8][8][2] = {0};
    dp[0][0][0] = (seq_t)1;

    for (idx_t r = 0; r < n; ++r)
    {
        /* clear newdp */
        for (int p = 0; p < 8; ++p)
            for (int c = 0; c < 8; ++c)
                for (int v = 0; v < 2; ++v)
                    newdp[p][c][v] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev)
            for (int cur = 0; cur < 8; ++cur)
                for (int viol = 0; viol < 2; ++viol)
                {
                    seq_t ways = dp[prev][cur][viol];
                    if (ways == (seq_t)0) continue;
                    for (int nxt = 0; nxt < 8; ++nxt)
                    {
                        uint8_t add = viol_table[prev][cur][nxt];
                        uint8_t nv = (uint8_t)(viol + add);
                        if (nv > 1) continue;                 /* more than one violation */
                        newdp[cur][nxt][nv] += ways;
                    }
                }

        /* swap dp and newdp for next iteration */
        for (int p = 0; p < 8; ++p)
            for (int c = 0; c < 8; ++c)
                for (int v = 0; v < 2; ++v)
                    dp[p][c][v] = newdp[p][c][v];
    }

    /* final step: evaluate the last real row against a sentinel bottom row (0) */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev)
        for (int cur = 0; cur < 8; ++cur)
            for (int viol = 0; viol < 2; ++viol)
            {
                seq_t ways = dp[prev][cur][viol];
                if (ways == (seq_t)0) continue;
                uint8_t add = viol_table[prev][cur][0];
                uint8_t nv = (uint8_t)(viol + add);
                if (nv == 1)                      /* exactly one violation in the whole array */
                    result += ways;
            }

    return result;
}
