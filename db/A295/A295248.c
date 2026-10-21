/*
 * A295248  Number of nX3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 3 1s.
 * https://oeis.org/A295248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed with a dynamic‑programming recursion over the
 *            three‑column binary matrix. The state is a pair of consecutive
 *            rows, and a transition is allowed only when all 1‑cells in the
 *            upper of the pair satisfy the adjacency rule using the preceding
 *            row, the row itself, and the following row. After p...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool valid_transition(int up, int cur, int down)
{
    /* For each column check the adjacency count of 1‑cells
       in the 'cur' row, where 'up' and 'down' are the rows
       immediately above and below cur. */
    for (int col = 0; col < 3; ++col)
    {
        if (cur & (1 << col))
        {
            int cnt = 0;
            /* vertical neighbors */
            if (up   & (1 << col)) ++cnt;
            if (down & (1 << col)) ++cnt;
            /* horizontal neighbors within same row */
            if (col > 0     && (cur & (1 << (col - 1)))) ++cnt;
            if (col < 2     && (cur & (1 << (col + 1)))) ++cnt;
            /* allowed counts: 0, 1, or 3 */
            if (!(cnt == 0 || cnt == 1 || cnt == 3))
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* dp[up][cur] = number of ways to produce rows up...cur
       when a valid transition has been made from the previous
       pair (prev_up, up) to (up, cur).  The table size is 8×8
       because a 3‑column row can be represented by a 3‑bit mask. */
    static seq_t dp_old[8][8];
    static seq_t dp_new[8][8];

    /* initial state: after first row, the top sentinel row is all 0. */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp_old[i][j] = 0;
    for (int p = 0; p < 8; ++p)
        dp_old[0][p] = 1;            /* state (0,p) after first row */

    /* special case n==1 (no further rows to process) */
    if (n == 1)
    {
        seq_t ans = 0;
        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur)
            {
                seq_t val = dp_old[up][cur];
                if (val == 0)
                    continue;
                if (valid_transition(up, cur, 0))
                    ans += val;
            }
        return ans;
    }

    /* process rows 2 … n.  After each step dp_new holds counts for
       pairs (prev, cur).  Then dp_new is copied back to dp_old. */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_new[i][j] = 0;

        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur)
            {
                seq_t val = dp_old[up][cur];
                if (val == 0)
                    continue;
                for (int next = 0; next < 8; ++next)
                {
                    if (valid_transition(up, cur, next))
                        dp_new[cur][next] += val;
                }
            }

        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_old[i][j] = dp_new[i][j];
    }

    /* finalize: the last row must satisfy the rule with a bottom
       sentinel row of 0s. */
    seq_t ans = 0;
    for (int up = 0; up < 8; ++up)
        for (int cur = 0; cur < 8; ++cur)
        {
            seq_t val = dp_old[up][cur];
            if (val == 0)
                continue;
            if (valid_transition(up, cur, 0))
                ans += val;
        }
    return ans;
}
