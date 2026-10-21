/*
 * A298707  Number of nX3 0..1 arrays with every element equal to 1, 3, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute A298707: number of n×3 0/1 arrays with
 * every element equal to 1, 3, 4, 6 or 7 king‑move adjacent elements,
 * with the upper‑left element equal to 0.
 *
 * The algorithm enumerates all 3‑bit rows (0–7), checks the
 * neighbour‑count condition on each cell, and runs dynamic
 * programming over the rows.  It works for any integer size
 * used for seq_t (unsigned integer or arbitrary‑precision).
 */
#include <stdbool.h>

static const bool allowed[9] = {
    false, /* 0 */
    true,  /* 1 */
    false, /* 2 */
    true,  /* 3 */
    true,  /* 4 */
    false, /* 5 */
    true,  /* 6 */
    true,  /* 7 */
    false  /* 8 */
};

static const int NO_ROW = 8;   /* sentinel for missing neighbour row */

/*
 * Return true if row 'curr' satisfies the neighbour count condition
 * when the upper row is 'prev' and the lower row is 'next'.
 * If prev or next is NO_ROW, that row is omitted from the checks.
 */
static bool row_valid(int prev, int curr, int next)
{
    for (int col = 0; col < 3; ++col) {
        int curbit = (curr >> (2 - col)) & 1;
        int count  = 0;
        for (int di = -1; di <= 1; ++di) {
            for (int dj = -1; dj <= 1; ++dj) {
                if (di == 0 && dj == 0) continue;
                int r = di, c = col + dj;
                if (c < 0 || c > 2) continue;
                int pat;
                if (r == -1) {
                    if (prev == NO_ROW) continue;
                    pat = prev;
                } else if (r == 0) {
                    pat = curr;
                } else { /* r == 1 */
                    if (next == NO_ROW) continue;
                    pat = next;
                }
                int nb = (pat >> (2 - c)) & 1;
                if (nb == curbit) ++count;
            }
        }
        if (!allowed[count]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive indices */

    /* Single row case (n == 1) */
    if (n == 1) {
        seq_t res = 0;
        for (int r0 = 0; r0 < 8; ++r0) {
            if (r0 & 4) continue;            /* upper‑left (bit 2) must be 0 */
            if (row_valid(NO_ROW, r0, NO_ROW))
                res += 1;
        }
        return res;
    }

    /* DP arrays: dp[prev][curr] = number of ways for the rows processed
       up to the current position with the last two rows being 'prev' and 'curr'.
       For n >= 2, we initialise dp with the first two rows. */
    seq_t dp[8][8];
    seq_t nextdp[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;

    for (int r0 = 0; r0 < 8; ++r0) {
        if (r0 & 4) continue;                /* upper‑left must be 0 */
        for (int r1 = 0; r1 < 8; ++r1) {
            if (row_valid(NO_ROW, r0, r1))
                dp[r0][r1] += 1;
        }
    }

    /* a(2) */
    if (n == 2) {
        seq_t res = 0;
        for (int prev = 0; prev < 8; ++prev)
            for (int curr = 0; curr < 8; ++curr)
                if (dp[prev][curr] && row_valid(prev, curr, NO_ROW))
                    res += dp[prev][curr];
        return res;
    }

    /* For n > 2, extend DP row by row */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero nextdp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                nextdp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int next = 0; next < 8; ++next) {
                    if (row_valid(prev, curr, next))
                        nextdp[curr][next] += ways;
                }
            }
        }

        /* copy nextdp into dp for the next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = nextdp[i][j];

        /* If this is the final row, compute a(n) */
        if (row == n) {
            seq_t ans = 0;
            for (int prev = 0; prev < 8; ++prev)
                for (int curr = 0; curr < 8; ++curr)
                    if (dp[prev][curr] && row_valid(prev, curr, NO_ROW))
                        ans += dp[prev][curr];
            return ans;
        }
    }

    /* Should never reach here */
    return 0;
}
