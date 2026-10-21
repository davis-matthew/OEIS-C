/*
 * A297429  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297429: Number of n×6 binary arrays where every 1 has exactly
   two neighbors among its six horizontally, diagonally, or antidiagonally
   adjacent cells.  The values are computed by dynamic programming over
   rows.  The recurrence depends on the previous two rows; only the last
   two rows are stored at each step.  The algorithm runs in O(n·4096)
   time and uses O(4096) memory. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Check whether a row `mid` satisfies the adjacency rule given the
   row above `prev` and the row below `next`.  All indices are 0‑based. */
static bool valid_row(idx_t prev, idx_t mid, idx_t next)
{
    for (int j = 0; j < 6; ++j) {
        if ((mid >> j) & 1) {
            int cnt = 0;
            /* horizontal neighbours within the row */
            if (j > 0     && ((mid >> (j - 1)) & 1)) ++cnt;
            if (j < 5     && ((mid >> (j + 1)) & 1)) ++cnt;
            /* diagonal neighbours from the row above */
            if (j > 0     && ((prev >> (j - 1)) & 1)) ++cnt;
            if (j < 5     && ((prev >> (j + 1)) & 1)) ++cnt;
            /* diagonal neighbours from the row below */
            if (j > 0     && ((next >> (j - 1)) & 1)) ++cnt;
            if (j < 5     && ((next >> (j + 1)) & 1)) ++cnt;
            if (cnt != 2) return false;
        }
    }
    return true;
}

/* Pre‑compute a transition table.  trans[(a << 6) | b] is a 64‑bit mask
   whose k‑th bit is 1 iff a row `k` together with rows `a` and `b`
   satisfies the rule for the middle row. */
static void init_trans(uint64_t *trans)
{
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            uint64_t bits = 0;
            for (int c = 0; c < 64; ++c) {
                if (valid_row(a, b, c))
                    bits |= ((uint64_t)1 << c);
            }
            trans[(a << 6) | b] = bits;
        }
    }
}

seq_t a(idx_t n)
{
    static bool trans_inited = false;
    static uint64_t trans[4096];
    if (!trans_inited) {
        init_trans(trans);
        trans_inited = true;
    }

    if (n == 0) return 0;                /* not part of sequence but safe */
    if (n == 1) {                        /* single row, no neighbours below */
        seq_t ans = 0;
        for (int m = 0; m < 64; ++m)
            if (valid_row(0, (idx_t)m, 0))
                ans += 1;
        return ans;
    }

    /* Dynamic programming over rows.  dp[cur][state] holds the number of
       ways to realise the last two rows represented by state.  state
       encodes (prev_row << 6) | cur_row. */
    static seq_t dp[2][4096];
    int cur = 0, nxt = 1;
    for (int i = 0; i < 4096; ++i) dp[cur][i] = 0;

    /* Initialise with the first two rows (prev = 0). */
    for (int m0 = 0; m0 < 64; ++m0) {
        for (int m1 = 0; m1 < 64; ++m1) {
            if (valid_row(0, (idx_t)m0, (idx_t)m1))
                dp[cur][(m0 << 6) | m1] += 1;
        }
    }

    /* Process rows 2 .. n-1 (0‑based).  Each step validates the middle
       row of the three consecutive rows. */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < 4096; ++i) dp[nxt][i] = 0;
        for (int state = 0; state < 4096; ++state) {
            seq_t cnt = dp[cur][state];
            if (!cnt) continue;
            int prev = state >> 6;          /* row above the middle */
            int currow = state & 63;        /* middle row */
            uint64_t bits = trans[(prev << 6) | currow];
            while (bits) {
                int next = (int)__builtin_ctzll(bits);
                bits &= bits - 1;
                dp[nxt][(currow << 6) | next] += cnt;
            }
        }
        /* swap cur and nxt */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Final verification of the last row (no row below). */
    seq_t ans = 0;
    for (int state = 0; state < 4096; ++state) {
        seq_t cnt = dp[cur][state];
        if (!cnt) continue;
        int prev = state >> 6;      /* second‑last row */
        int currow = state & 63;    /* last row */
        if (valid_row(prev, (idx_t)currow, 0))
            ans += cnt;
    }
    return ans;
}
