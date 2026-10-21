/*
 * A282478  Number of nX5 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where exactly two 1s have >3
 *            king‑move neighbors. DP over rows with state (previous two rows,
 *            violation count ≤ 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Precompute violation counts for each triad of rows. */
    static uint8_t viol[32][32][32];
    static bool init = false;
    if (!init) {
        for (int up = 0; up < 32; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                for (int down = 0; down < 32; ++down) {
                    int vcnt = 0;
                    for (int col = 0; col < 5; ++col) {
                        if (!(mid & (1 << col)))
                            continue;   /* cell is 0 */
                        int neigh = 0;
                        /* horizontal neighbours */
                        if (col > 0 && (mid & (1 << (col - 1)))) ++neigh;
                        if (col < 4 && (mid & (1 << (col + 1)))) ++neigh;
                        /* vertical and diagonal neighbours */
                        if (up   & (1 << col))             ++neigh;
                        if (down & (1 << col))             ++neigh;
                        if (col > 0 && (up   & (1 << (col - 1)))) ++neigh;
                        if (col < 4 && (up   & (1 << (col + 1)))) ++neigh;
                        if (col > 0 && (down & (1 << (col - 1)))) ++neigh;
                        if (col < 4 && (down & (1 << (col + 1)))) ++neigh;
                        if (neigh > 3) ++vcnt;
                    }
                    viol[up][mid][down] = (uint8_t)vcnt;
                }
            }
        }
        init = true;
    }

    static seq_t dp[32][32][3];
    static seq_t dp_next[32][32][3];

    /* zero dp array */
    memset(dp, 0, sizeof(dp));

    /* initialise first row: previous row is all zeros */
    for (int row = 0; row < 32; ++row)
        dp[0][row][0] = (seq_t)1;

    /* Process rows 2 .. n */
    for (idx_t step = 1; step < n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int up = 0; up < 32; ++up) {
            for (int curr = 0; curr < 32; ++curr) {
                for (int v = 0; v < 3; ++v) {
                    seq_t val = dp[up][curr][v];
                    if (val == (seq_t)0) continue;
                    for (int nxt = 0; nxt < 32; ++nxt) {
                        uint8_t addv = viol[up][curr][nxt];
                        int nv = v + addv;
                        if (nv <= 2)
                            dp_next[curr][nxt][nv] += val;
                    }
                }
            }
        }

        /* copy dp_next into dp for next iteration */
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Validate last row against all-zero sentinel row */
    seq_t result = (seq_t)0;
    for (int up = 0; up < 32; ++up) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int v = 0; v < 3; ++v) {
                seq_t val = dp[up][curr][v];
                if (val == (seq_t)0) continue;
                int final_v = v + viol[up][curr][0];
                if (final_v == 2)
                    result += val;
            }
        }
    }
    return result;
}
