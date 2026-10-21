/*
 * A278173  Number of nX4 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) (1,-1)
 *          or (1,0), with upper left element zero.
 * https://oeis.org/A278173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A278173 – Number of n×4 0/1 arrays with the given neighbour rule
 *
 *  Computation: transfer‑matrix dynamic programming over rows.
 *  For each row we keep its previous two rows.  When a new row is
 *  added, the middle row of the three becomes fully verified
 *  (it sees its up and down neighbors).  After the final row
 *  is processed, the last row is verified with its upper row only.
 *
 *  Complexity: O(n·16³) ≈ 4·10⁴ operations per 10000 rows.
 *  Memory: 256 64‑bit integers.  All arithmetic uses seq_t.
 */

#include <stdbool.h>

enum { WIDTH = 4, M = 1 << WIDTH };

/* Check whether every element of `cur` satisfies the condition
 * when there is a row `up` above and a row `down` below.
 */
static bool row_ok(int cur, bool hasUp, int up, bool hasDown, int down)
{
    for (int j = 0; j < WIDTH; ++j) {
        int val = (cur >> j) & 1;
        bool eq = false, ne = false;

        /* neighbour above */
        if (hasUp) {
            int u   = (up >> j) & 1;
            if (u == val) eq = true; else ne = true;
            if (j + 1 < WIDTH) {
                int u2  = (up >> (j + 1)) & 1;
                if (u2 == val) eq = true; else ne = true;
            }
        }
        /* left neighbour */
        if (j > 0) {
            int l = (cur >> (j - 1)) & 1;
            if (l == val) eq = true; else ne = true;
        }
        /* right neighbour */
        if (j + 1 < WIDTH) {
            int r = (cur >> (j + 1)) & 1;
            if (r == val) eq = true; else ne = true;
        }
        /* neighbour below */
        if (hasDown) {
            int d  = (down >> j) & 1;
            if (d == val) eq = true; else ne = true;
            if (j > 0) {
                int d2 = (down >> (j - 1)) & 1;
                if (d2 == val) eq = true; else ne = true;
            }
        }
        if (!eq || !ne) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* dp[prev][cur] : counts of arrays where `prev` is the
     * previous row (already verified with its up neighbour),
     * `cur` is the current row (waiting to be verified).
     */
    seq_t dp[M][M] = {0};
    seq_t nxt[M][M];

    /* initialise for the first two rows (n>=2) */
    for (int r0 = 0; r0 < M; ++r0) {
        if ((r0 & 1) == 0) {               /* upper‑left fixed 0 */
            for (int r1 = 0; r1 < M; ++r1) {
                if (row_ok(r0, false /*no up*/, 0, true /*has down*/, r1))
                    dp[r0][r1] = 1;
            }
        }
    }

    /* special handling of n=1: only the top row exists */
    if (n == 1) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < M; ++r0) {
            if ((r0 & 1) == 0 && row_ok(r0, false, 0, false, 0))
                ans += 1;
        }
        return ans;
    }

    /* for n=2 we only need to verify the last row */
    if (n == 2) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < M; ++r0) {
            if ((r0 & 1) == 0) {
                for (int r1 = 0; r1 < M; ++r1) {
                    seq_t cnt = dp[r0][r1];
                    if (cnt && row_ok(r1, true /*has up*/, r0, false, 0))
                        ans += cnt;
                }
            }
        }
        return ans;
    }

    /* n > 2 : add rows one by one */
    for (idx_t row = 2; row < n; ++row) {
        /* reset nxt */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                nxt[i][j] = 0;

        for (int pprev = 0; pprev < M; ++pprev) {
            for (int p = 0; p < M; ++p) {
                seq_t cnt = dp[pprev][p];
                if (!cnt) continue;
                for (int cur = 0; cur < M; ++cur) {
                    if (row_ok(p, true /*up*/, pprev, true /*down*/, cur))
                        nxt[p][cur] += cnt;   /* new state: (p,cur) */
                }
            }
        }

        /* copy nxt to dp */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* final row (last) must satisfy its constraints with its upper row */
    seq_t ans = 0;
    for (int pprev = 0; pprev < M; ++pprev) {
        for (int p = 0; p < M; ++p) {
            seq_t cnt = dp[pprev][p];
            if (!cnt) continue;
            if (row_ok(p, true /*has up*/, pprev, false, 0))
                ans += cnt;
        }
    }
    return ans;
}
