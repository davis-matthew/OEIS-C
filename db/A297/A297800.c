/*
 * A297800  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 5 neighboring 1s.
 * https://oeis.org/A297800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of n×6 0..1 arrays with every 1 horizontally, diagonally
 * or antidiagonally adjacent to 2, 3 or 5 neighboring 1s; column 6 of
 * A297802.  Computation done by a DP over rows using a three‑row sliding
 * window.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: test that every 1 in row `mid` has 2, 3 or 5 neighbours
 * among the six possible directions (horizontal and both diagonal directions).
 * up and down are the rows immediately above and below `mid`. */
static inline bool row_valid(int up, int mid, int down)
{
    for (int j = 0; j < 6; ++j) {
        if (mid & (1 << j)) {
            int cnt = 0;
            /* horizontal neighbours */
            if (j > 0   && (mid & (1 << (j - 1)))) ++cnt;
            if (j < 5   && (mid & (1 << (j + 1)))) ++cnt;
            /* diagonal neighbours from the upper row */
            if (j > 0   && (up   & (1 << (j - 1)))) ++cnt;
            if (j < 5   && (up   & (1 << (j + 1)))) ++cnt;
            /* diagonal neighbours from the lower row */
            if (j > 0   && (down & (1 << (j - 1)))) ++cnt;
            if (j < 5   && (down & (1 << (j + 1)))) ++cnt;
            if (!(cnt == 2 || cnt == 3 || cnt == 5))
                return false;
        }
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset 1 – this value is unused */

    /* Pre‑compute compatibility masks: for each pair (up,mid) the 64‑bit
     * mask of all possible `down` rows that make `mid` valid. */
    static uint64_t compat[64][64];
    static bool compat_done = false;
    if (!compat_done) {
        for (int up = 0; up < 64; ++up)
            for (int mid = 0; mid < 64; ++mid) {
                uint64_t mask = 0;
                for (int down = 0; down < 64; ++down)
                    if (row_valid(up, mid, down))
                        mask |= (1ULL << down);
                compat[up][mid] = mask;
            }
        compat_done = true;
    }

    /* DP arrays: dp[prevprev][prev] holds number of ways to finish up to
     * the current row with preceding rows 'prevprev' and 'prev'. */
    seq_t dp[64][64] = {0};
    seq_t nextdp[64][64] = {0};

    /* Initialise for the first row (prevprev = 0). Any row `first` that
     * is valid with no row above and the next row `second` will become
     * a starting state. */
    for (int first = 0; first < 64; ++first)
        for (int second = 0; second < 64; ++second)
            if (row_valid(0, first, second))
                dp[first][second] = 1;

    /* If only one row, handle separately because the final row has no
     * lower neighbour. */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 64; ++r)
            if (row_valid(0, r, 0))
                ++ans;
        return ans;
    }

    /* Process rows 3..n (0‑based step counts the new row index). */
    for (idx_t step = 2; step < n; ++step) {
        /* clear nextdp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                nextdp[i][j] = 0;

        /* transition */
        for (int up = 0; up < 64; ++up)
            for (int mid = 0; mid < 64; ++mid) {
                seq_t ways = dp[up][mid];
                if (ways == 0) continue;
                uint64_t mask = compat[up][mid];
                while (mask) {
                    int next = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    nextdp[mid][next] += ways;
                }
            }

        /* copy back */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* Final state: last row `mid` has no lower neighbour (down=0). */
    seq_t answer = 0;
    for (int up = 0; up < 64; ++up)
        for (int mid = 0; mid < 64; ++mid) {
            seq_t ways = dp[up][mid];
            if (ways == 0) continue;
            if (row_valid(up, mid, 0))
                answer += ways;
        }

    return answer;
}
