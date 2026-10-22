/*
 * A302066  Number of nX5 0..1 arrays with every element equal to 0, 1, 2 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays (upper-left element must be
 *            0) such that every element has 0, 1, 2 or 4 horizontally or
 *            antidiagonally adjacent 1’s. This implementation uses dynamic
 *            programming over the rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

#define NUM_MASKS    32        /* 2⁵ possible column values */
#define NULL_MASK    32        /* special value meaning “no row” */

static bool tables_built = false;
static seq_t dp[NUM_MASKS + 1][NUM_MASKS];
static seq_t newdp[NUM_MASKS + 1][NUM_MASKS];
static bool ok_table[NUM_MASKS + 1][NUM_MASKS][NUM_MASKS];
static bool final_ok[NUM_MASKS + 1][NUM_MASKS];
static int  next_masks[NUM_MASKS + 1][NUM_MASKS][NUM_MASKS];
static int  next_count[NUM_MASKS + 1][NUM_MASKS];

seq_t a(idx_t n)
{
    /* Pre‑compute transition tables the first time the function is called. */
    if (!tables_built) {
        for (int pp = 0; pp <= NUM_MASKS; ++pp) {
            for (int p = 0; p < NUM_MASKS; ++p) {
                int cnt = 0;
                for (int c = 0; c < NUM_MASKS; ++c) {
                    bool valid = true;
                    for (int j = 0; j < 5; ++j) {
                        int b = (p >> j) & 1;
                        int sum = 0;
                        /* left neighbour */
                        if (j > 0 && ((p >> (j - 1)) & 1) == b) sum++;
                        /* right neighbour */
                        if (j < 4 && ((p >> (j + 1)) & 1) == b) sum++;
                        /* up‑right neighbour */
                        if (pp != NULL_MASK && j < 4 &&
                            ((pp >> (j + 1)) & 1) == b) sum++;
                        /* down‑left neighbour */
                        if (c != NULL_MASK && j > 0 &&
                            ((c >> (j - 1)) & 1) == b) sum++;
                        if (!(sum == 0 || sum == 1 || sum == 2 || sum == 4)) {
                            valid = false;
                            break;
                        }
                    }
                    ok_table[pp][p][c] = valid;
                    if (valid) next_masks[pp][p][cnt++] = c;
                }
                next_count[pp][p] = cnt;

                /* Check the bottom row constraint (no down‑left neighbour). */
                bool last_ok = true;
                for (int j = 0; j < 5; ++j) {
                    int b = (p >> j) & 1;
                    int sum = 0;
                    if (j > 0 && ((p >> (j - 1)) & 1) == b) sum++;
                    if (j < 4 && ((p >> (j + 1)) & 1) == b) sum++;
                    if (pp != NULL_MASK && j < 4 &&
                        ((pp >> (j + 1)) & 1) == b) sum++;
                    /* no down‑left neighbour */
                    if (!(sum == 0 || sum == 1 || sum == 2 || sum == 4)) {
                        last_ok = false;
                        break;
                    }
                }
                final_ok[pp][p] = last_ok;
            }
        }
        tables_built = true;
    }

    /* Initialise DP with the first row (upper‑left element must be 0). */
    memset(dp, 0, sizeof(dp));
    for (int first = 0; first < NUM_MASKS; ++first) {
        if ((first & 1) == 0)           /* bit 0 is the upper‑left cell */
            dp[NULL_MASK][first] = (seq_t)1;
    }

    /* Special case: n = 1 – finish by checking the last‑row constraints. */
    if (n == 1) {
        seq_t result = 0;
        for (int first = 0; first < NUM_MASKS; ++first)
            if ((first & 1) == 0 && final_ok[NULL_MASK][first])
                result += dp[NULL_MASK][first];
        return result;
    }

    /* Process rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        memset(newdp, 0, sizeof(newdp));
        for (int pp = 0; pp <= NUM_MASKS; ++pp) {
            for (int p = 0; p < NUM_MASKS; ++p) {
                seq_t ways = dp[pp][p];
                if (ways == 0) continue;
                int cnt = next_count[pp][p];
                for (int t = 0; t < cnt; ++t) {
                    int cur = next_masks[pp][p][t];
                    newdp[p][cur] += ways;
                }
            }
        }
        memcpy(dp, newdp, sizeof(dp));
    }

    /* Final validation: the last row must satisfy its constraints. */
    seq_t result = 0;
    for (int pp = 0; pp <= NUM_MASKS; ++pp) {
        for (int p = 0; p < NUM_MASKS; ++p) {
            if (dp[pp][p] == 0) continue;
            if (final_ok[pp][p])
                result += dp[pp][p];
        }
    }

    return result;
}
