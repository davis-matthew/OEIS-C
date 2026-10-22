/*
 * A301886  Number of 3Xn 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A301886 is computed by a transfer‑matrix dynamic program. For a
 *            3×n binary array with the upper‑left entry fixed to 0, every
 *            cell must not have exactly 2 or 4 neighbours (horizontally or
 *            anti‑diagonally) that are equal to its own value. The DP
 *            processes columns left to right, keeping the last two...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard support for the boolean type. */
#include <stdbool.h>

/* Number of possible 3‑bit column patterns. */
static const int PAT_CNT   = 8;   /* 0 … 7 */
static const int SENTINEL  = PAT_CNT;   /* sentinel index for the left boundary */

/* -------------------------------------------------------------------- */
/* Check if column `cur` satisfies the rule when it has a left neighbour
   `prev` (if prev != SENTINEL) and a right neighbour `nxt`.            */
static bool valid_between(int prev, int cur, int nxt)
{
    for (int r = 0; r < 3; ++r) {
        int val = (cur >> r) & 1;   /* value of the cell (r, current column) */
        int eq  = 0;                /* number of equal neighbours */

        /* left neighbour and down‑left neighbour come from `prev`. */
        if (prev != SENTINEL) {
            int left = (prev >> r) & 1;
            if (left == val) ++eq;
            if (r < 2) {              /* down‑left exists only if r < 2 */
                int dl = (prev >> (r + 1)) & 1;
                if (dl == val) ++eq;
            }
        }

        /* right neighbour and up‑right neighbour come from `nxt`. */
        int right = (nxt >> r) & 1;
        if (right == val) ++eq;
        if (r > 0) {                  /* up‑right exists only if r > 0 */
            int ur = (nxt >> (r - 1)) & 1;
            if (ur == val) ++eq;
        }

        if (eq == 2 || eq == 4) return false;   /* forbidden */
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* Check if the last column `cur` satisfies the rule when it has only a
   left neighbour `prev` (if prev != SENTINEL).  No right neighbour
   exists.                                                            */
static bool valid_last(int prev, int cur)
{
    for (int r = 0; r < 3; ++r) {
        int val = (cur >> r) & 1;
        int eq  = 0;

        if (prev != SENTINEL) {
            int left = (prev >> r) & 1;
            if (left == val) ++eq;
            if (r < 2) {
                int dl = (prev >> (r + 1)) & 1;
                if (dl == val) ++eq;
            }
        }
        /* no right or up‑right neighbour for the last column */

        if (eq == 2 || eq == 4) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* Main sequence generator.                                             */
seq_t a(idx_t n)
{
    /* DP state: dp[prev][cur] counts ways to form the processed part
       of the array with previous column pattern = prev and current
       column pattern = cur.                                              */
    seq_t dp[SENTINEL + 1][PAT_CNT];
    seq_t next[SENTINEL + 1][PAT_CNT];

    /* Initialise array to zero. */
    for (int i = 0; i <= SENTINEL; ++i)
        for (int j = 0; j < PAT_CNT; ++j)
            dp[i][j] = 0;

    /* First column: previous column is the left boundary sentinel.   */
    for (int p0 = 0; p0 < PAT_CNT; ++p0) {
        if (p0 & 1) continue;          /* top‑left element must be 0 */
        dp[SENTINEL][p0] = 1;
    }

    /* Process columns 1 .. n‑1.  Each iteration adds a new column and
       validates the middle column.                                  */
    for (idx_t col = 1; col < n; ++col) {
        /* Zero the temporary array. */
        for (int i = 0; i <= SENTINEL; ++i)
            for (int j = 0; j < PAT_CNT; ++j)
                next[i][j] = 0;

        /* Transitions.                                               */
        for (int prev = 0; prev <= SENTINEL; ++prev) {
            for (int cur = 0; cur < PAT_CNT; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                for (int nxt = 0; nxt < PAT_CNT; ++nxt) {
                    if (!valid_between(prev, cur, nxt)) continue;
                    next[cur][nxt] += ways;
                }
            }
        }

        /* Move to the next state. */
        for (int i = 0; i <= SENTINEL; ++i)
            for (int j = 0; j < PAT_CNT; ++j)
                dp[i][j] = next[i][j];
    }

    /* Final validation of the last column.  Sum counts that satisfy
       the rule when the right neighbour is missing.                 */
    seq_t ans = 0;
    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int cur = 0; cur < PAT_CNT; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways) continue;
            if (valid_last(prev, cur))
                ans += ways;
        }
    }
    return ans;
}
