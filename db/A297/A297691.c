/*
 * A297691  Number of n X 5 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 0, 2 or 4 neighboring 1's.
 * https://oeis.org/A297691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the number of n×5 binary arrays in which every 1 has
 * 0, 2, or 4 neighboring 1's among the six cells that are horizontally
 * adjacent (left/right) or diagonally adjacent (top-left, top-right,
 * bottom-left, bottom-right).  This is a finite–state DP over the rows.
 */

#include <stdint.h>
#include <stdbool.h>

static bool valid_row(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < 5; ++j)
    {
        if (!((b >> j) & 1u))
            continue;
        int cnt = 0;
        /* horizontal neighbours */
        if (j > 0)   cnt += (b >> (j - 1)) & 1u;
        if (j < 4)   cnt += (b >> (j + 1)) & 1u;
        /* diagonal neighbours */
        if (j > 0)   cnt += (a >> (j - 1)) & 1u;
        if (j < 4)   cnt += (a >> (j + 1)) & 1u;
        if (j > 0)   cnt += (c >> (j - 1)) & 1u;
        if (j < 4)   cnt += (c >> (j + 1)) & 1u;
        if (!(cnt == 0 || cnt == 2 || cnt == 4))
            return false;
    }
    return true;
}

static uint8_t trans_next[32][32][32];
static uint8_t trans_len[32][32];
static bool trans_built = false;

static void build_trans(void)
{
    for (int a = 0; a < 32; ++a)
    {
        for (int b = 0; b < 32; ++b)
        {
            uint8_t len = 0;
            for (int c = 0; c < 32; ++c)
                if (valid_row((uint8_t)a, (uint8_t)b, (uint8_t)c))
                    trans_next[a][b][len++] = (uint8_t)c;
            trans_len[a][b] = len;
        }
    }
    trans_built = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!trans_built) build_trans();

    static seq_t dp[32][32];
    static seq_t nextdp[32][32];

    /* clear dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    if (n == 1)
    {
        seq_t tot = 0;
        for (int a = 0; a < 32; ++a)
            if (valid_row(0, (uint8_t)a, 0))
                tot += (seq_t)1;
        return tot;
    }

    /* initialise with the first two rows (row 1 = a, row 2 = b) */
    for (int a = 0; a < 32; ++a)
        for (int k = 0; k < trans_len[0][a]; ++k)
        {
            int b = trans_next[0][a][k];
            dp[a][b] = 1;
        }

    /* add rows 3 .. n */
    for (idx_t r = 3; r <= n; ++r)
    {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                nextdp[i][j] = 0;

        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
            {
                seq_t val = dp[a][b];
                if (!val) continue;
                for (int k = 0; k < trans_len[a][b]; ++k)
                {
                    int c = trans_next[a][b][k];
                    nextdp[b][c] += val;
                }
            }

        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* count final states where the last row satisfies the constraint with
       no row below (c = 0). */
    seq_t ans = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
        {
            seq_t val = dp[a][b];
            if (!val) continue;
            if (valid_row((uint8_t)a, (uint8_t)b, 0))
                ans += val;
        }
    return ans;
}
