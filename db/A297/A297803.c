/*
 * A297803  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 5 neighboring 1s.
 * https://oeis.org/A297803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computation by finite‑state DP on column patterns.
   For a 3×n binary matrix each 1 must have 2, 3 or 5 horizontal/diagonal
   neighbours (no vertical adjacency).  The DP transitions over
   overlapping 3‑bit column patterns. */

static bool good_tbl[8][8][8];
static bool good_tbl_ready = false;

static inline bool bit(int pat, int r) { return (pat >> r) & 1; }

/* Return true if the middle column ‘cur’ satisfies the adjacency rule
   when the left column is ‘prev’ and the right column is ‘next’. */
static bool good_for_patterns(int prev, int cur, int next)
{
    for (int r = 0; r < 3; ++r)
        if (bit(cur, r))
        {
            int cnt = 0;
            for (int dr = -1; dr <= 1; ++dr)
            {
                int rr = r + dr;
                if (rr < 0 || rr > 2) continue;
                if (bit(prev, rr)) ++cnt;   /* left neighbours */
                if (bit(next, rr)) ++cnt;   /* right neighbours */
            }
            if (!(cnt == 2 || cnt == 3 || cnt == 5))
                return false;
        }
    return true;
}

static void init_good_tbl(void)
{
    if (good_tbl_ready) return;
    for (int p = 0; p < 8; ++p)
        for (int c = 0; c < 8; ++c)
            for (int n = 0; n < 8; ++n)
                good_tbl[p][c][n] = good_for_patterns(p, c, n);
    good_tbl_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, harmless */

    init_good_tbl();

    static seq_t dp[64], np[64];
    for (int i = 0; i < 64; ++i) dp[i] = 0;

    /* start with left neighbour 0, current column = p */
    for (int p = 0; p < 8; ++p)
        dp[(0 << 3) | p] = 1;

    for (idx_t col = 1; col < n; ++col)
    {
        for (int i = 0; i < 64; ++i) np[i] = 0;
        for (int idx = 0; idx < 64; ++idx)
        {
            seq_t val = dp[idx];
            if (val == 0) continue;
            int prev = idx >> 3;
            int cur  = idx & 7;
            for (int next = 0; next < 8; ++next)
            {
                if (!good_tbl[prev][cur][next]) continue;
                int nidx = (cur << 3) | next;
                np[nidx] += val;
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = np[i];
    }

    seq_t ans = 0;
    for (int idx = 0; idx < 64; ++idx)
    {
        int prev = idx >> 3;
        int cur  = idx & 7;
        if (good_tbl[prev][cur][0]) ans += dp[idx];
    }
    return ans;
}
