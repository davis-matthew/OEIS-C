/*
 * A297698  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297698 – Number of 5×n binary arrays in which every 1 has
   0, 2 or 4 horizontally/diagonally/antidiagonally adjacent 1s.
   The function uses a transfer‑matrix DP that processes the
   columns one by one, keeping the two most recent columns.
   It works for any seq_t / idx_t type and runs in O(n·32²). */

#include <stdint.h>
#include <stdbool.h>

/* Check that every 1 in column 'b' has 0,2,4 neighbours
   among the *horizontal and diagonal* positions defined
   by the left column 'a' and right column 'c'. */
static inline bool valid_middle(uint32_t b, uint32_t a, uint32_t c)
{
    for (int r = 0; r < 5; ++r)
    {
        if (!((b >> r) & 1u))
            continue;                         /* only 1's matter */

        int cnt = 0;

        /* left neighbour */
        if ((a >> r) & 1u) ++cnt;
        /* right neighbour */
        if ((c >> r) & 1u) ++cnt;

        /* diagonal neighbours */
        if (r > 0     && ((a >> (r - 1)) & 1u)) ++cnt;   /* left‑up */
        if (r < 4     && ((a >> (r + 1)) & 1u)) ++cnt;   /* left‑down */
        if (r > 0     && ((c >> (r - 1)) & 1u)) ++cnt;   /* right‑up */
        if (r < 4     && ((c >> (r + 1)) & 1u)) ++cnt;   /* right‑down */

        if (!(cnt == 0 || cnt == 2 || cnt == 4))
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Precompute allowed next columns for each pair of previous columns. */
    static bool initialized = false;
    static uint32_t next_mask[32][32];
    if (!initialized)
    {
        for (uint32_t a_col = 0; a_col < 32; ++a_col)
            for (uint32_t b_col = 0; b_col < 32; ++b_col)
            {
                uint32_t mask = 0;
                for (uint32_t c_col = 0; c_col < 32; ++c_col)
                    if (valid_middle(b_col, a_col, c_col))
                        mask |= (1u << c_col);
                next_mask[a_col][b_col] = mask;
            }
        initialized = true;
    }

    /* DP tables: dp[prevprev][prev] holds counts for the last two columns. */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* Reset dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* Initial state: dummy left column (0) followed by first real column. */
    for (uint32_t col = 0; col < 32; ++col)
        dp[0][col] = 1;

    /* Process columns: step 2 corresponds to adding the second column,
       and step n+1 adds the dummy right column. */
    for (idx_t step = 2; step <= n + 1; ++step)
    {
        /* Clear new DP state. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (uint32_t a_col = 0; a_col < 32; ++a_col)
            for (uint32_t b_col = 0; b_col < 32; ++b_col)
            {
                seq_t val = dp[a_col][b_col];
                if (!val)
                    continue;
                uint32_t mask = next_mask[a_col][b_col];
                while (mask)
                {
                    uint32_t c_col = __builtin_ctz(mask);
                    ndp[b_col][c_col] += val;
                    mask &= mask - 1;              /* clear lowest set bit */
                }
            }

        /* Swap dp and ndp for next step. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all states where the last column is the dummy (0). */
    seq_t ans = 0;
    for (uint32_t a_col = 0; a_col < 32; ++a_col)
        ans += dp[a_col][0];

    return ans;
}
