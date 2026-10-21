/*
 * A299330  Number of nX4 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299330: # of n×4 0/1 arrays with upper‑left element 0 such
 * that for each cell the number of king‑move adjacent cells having the
 * same value is between 2 and 7 inclusive.
 *
 * Computation via a transfer‑matrix DP over rows:
 *   * For each triple of consecutive rows (prev, curr, next) we precompute
 *     whether the middle row satisfies the adjacency constraints.
 *   * We then walk down the rows, keeping a state (prev, curr) and
 *     extending it with a new row next if it keeps curr valid.
 *   * After the last row we require its validity with a bottom sentinel.
 * Initial values: a(1)=0, a(2)=6, a(3)=36, a(4)=263, a(5)=2537 ...
 * Complexity: O(n·17·16·16) ≈ 43 000 n elementary operations.
 *
 * No external dependencies beyond standard <stdint.h> and <stdbool.h>.
 * seq_t and idx_t are supplied by the build.
 */
#include <stdint.h>
#include <stdbool.h>

static const int SENTINEL = 16;          /* sentinel row (no neighbors) */
static const int ROWS     = 16;           /* 4 columns → 2⁴ = 16 masks */

static bool valid[17][16][17];
static bool valid_computed = false;

/* Compute validity of a middle row given previous and next row masks.
 * The sentinel value means "no row" – no neighbors are counted from that side. */
static void compute_valid(void)
{
    for (int prevIdx = 0; prevIdx <= SENTINEL; ++prevIdx) {
        for (int currIdx = 0; currIdx < ROWS; ++currIdx) {
            for (int nextIdx = 0; nextIdx <= SENTINEL; ++nextIdx) {
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    int cellVal = (currIdx >> col) & 1;
                    int cnt = 0;
                    for (int dr = -1; dr <= 1; ++dr) {
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue;
                            int col2 = col + dc;
                            if (col2 < 0 || col2 > 3) continue;

                            bool exists = true;
                            int neighborBit = 0;
                            if (dr == -1) {
                                if (prevIdx == SENTINEL) exists = false;
                                else neighborBit = (prevIdx >> col2) & 1;
                            } else if (dr == 0) {
                                neighborBit = (currIdx >> col2) & 1;
                            } else {                /* dr == 1 */
                                if (nextIdx == SENTINEL) exists = false;
                                else neighborBit = (nextIdx >> col2) & 1;
                            }

                            if (exists && neighborBit == cellVal) ++cnt;
                        }
                    }
                    if (cnt < 2 || cnt > 7) { ok = false; break; }
                }
                valid[prevIdx][currIdx][nextIdx] = ok;
            }
        }
    }
    valid_computed = true;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (!valid_computed) compute_valid();

    if (n == 0) return (seq_t)0;   /* undefined, but defensive */

    /* DP arrays: dp[prev][curr] holds counts after having processed
     * up to some row where curr is the last processed row. */
    seq_t dp[17][16]  = {0};
    seq_t dp2[17][16] = {0};

    /* initial row: prev is sentinel, curr is first row mask
     * with top‑left element (bit 0) equal to 0 */
    for (int first = 0; first < ROWS; ++first)
        if ((first & 1) == 0) dp[SENTINEL][first] = 1;

    /* process rows 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero dp2 */
        for (int i = 0; i < 17; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp2[i][j] = 0;

        for (int prev = 0; prev <= SENTINEL; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                for (int next = 0; next < ROWS; ++next) {
                    if (!valid[prev][curr][next]) continue;
                    dp2[curr][next] += val;
                }
            }
        }

        /* copy dp2 to dp for next iteration */
        for (int i = 0; i < 17; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = dp2[i][j];
    }

    /* final row validation with bottom sentinel */
    seq_t total = 0;
    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            seq_t val = dp[prev][curr];
            if (val == 0) continue;
            if (valid[prev][curr][SENTINEL]) total += val;
        }
    }

    return total;
}
