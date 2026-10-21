/*
 * A296646  Number of n X 3 0..1 arrays with each 1 adjacent to 0, 1 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays such that every 1 has * 0, 1,
 *            or 3 king‑move neighboring 1s. The values are computed by a *
 *            transfer‑matrix DP that enumerates all 3‑bit row patterns. * For
 *            n ≥ 2, the DP first enforces the constraint for the first row *
 *            when the second row is added. Every subse...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of seq_t and idx_t types provided by the build. */

/* Returns true iff the central row pattern 'curr' satisfies the
 * adjacency rule when surrounded by 'prev' (above) and 'next' (below).
 * All patterns are 3‑bit values 0..7.  Missing rows are represented
 * as the zero pattern. */
static bool valid_row(idx_t prev, idx_t curr, idx_t next)
{
    /* For each column j (0..2) with a 1 in the current row, count
     * its king‑move neighbors among the three rows. */
    for (int j = 0; j < 3; ++j) {
        if (((curr >> j) & 1) == 0) continue;
        int neigh = 0;
        for (int dj = -1; dj <= 1; ++dj) {
            int cj = j + dj;
            if (cj < 0 || cj > 2) continue;
            for (int di = -1; di <= 1; ++di) {
                if (di == 0 && dj == 0) continue;
                idx_t row = 0;
                if (di == -1) row = prev;
                else if (di == 0) row = curr;
                else if (di == 1) row = next;
                if (((row >> cj) & 1) != 0) ++neigh;
            }
        }
        if (neigh != 0 && neigh != 1 && neigh != 3) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, but harmless */

    /* Special case n = 1: only top and bottom missing. */
    if (n == 1) {
        seq_t total = 0;
        for (idx_t p = 0; p < 8; ++p)
            if (valid_row(0, p, 0))
                ++total;
        return total;
    }

    /* DP arrays: dp[prev][curr] holds count of ways for current
     * two‑row prefix.  The values fit in seq_t. */
    seq_t dp[8][8];
    seq_t ndp[8][8];
    /* initialize all entries to zero */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;

    /* Initial step: choose first row 'r1' and second row 'r2'.
     * Enforce constraint for the first row using the missing
     * above row (0) and the second row. */
    for (idx_t r1 = 0; r1 < 8; ++r1) {
        for (idx_t r2 = 0; r2 < 8; ++r2) {
            if (valid_row(0, r1, r2))
                dp[r1][r2] = 1;
        }
    }

    /* Iterate over remaining rows 3 .. n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* set ndp to zero */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int next = 0; next < 8; ++next) {
                    if (valid_row(prev, curr, next))
                        ndp[curr][next] += ways;
                }
            }
        }
        /* copy ndp back to dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final check: validate the last row against a non‑existent
     * below row (all zeros). */
    seq_t total = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            seq_t ways = dp[prev][curr];
            if (ways == 0) continue;
            if (valid_row(prev, curr, 0))
                total += ways;
        }
    }
    return total;
}
