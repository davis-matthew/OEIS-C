/*
 * A295549  Number of n X 6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 3 or 4 1's.
 * https://oeis.org/A295549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A295549: number of n×6 0/1 arrays in which each 1 is adjacent
   horizontally or vertically to 1, 3, or 4 other 1’s.  The algorithm
   precomputes horizontal neighbour counts and uses dynamic programming
   over the rows of the array. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Anonymous structs are not usable in a type‑unique manner across the
   file, so we give the adjacency list a typedef. */
typedef struct {
    uint8_t cnt;
    uint8_t list[64];
} adj_t;

static bool init_done = false;

/* Pre‑computed helper tables */
static bool single_valid[64];
static bool top_valid[64][64];
static bool bottom_valid[64][64];
static uint8_t horiz_neigh[64][6];
static adj_t next_adj[64 * 64];

/* Initialise the tables once. */
static void precompute(void)
{
    /* Horizontal neighbour counts for each row mask. */
    for (int mask = 0; mask < 64; ++mask) {
        for (int col = 0; col < 6; ++col) {
            if (!(mask >> col & 1)) {
                horiz_neigh[mask][col] = 0;
                continue;
            }
            int cnt = 0;
            if (col > 0 && ((mask >> (col - 1)) & 1)) ++cnt;
            if (col < 5 && ((mask >> (col + 1)) & 1)) ++cnt;
            horiz_neigh[mask][col] = (uint8_t)cnt;
        }
    }

    /* Single row validity (no vertical neighbours). */
    for (int mask = 0; mask < 64; ++mask) {
        bool ok = true;
        for (int col = 0; col < 6 && ok; ++col) {
            if (!(mask >> col & 1)) continue;
            int h = horiz_neigh[mask][col];
            if (h != 1) ok = false;          /* must be exactly one horizontal neighbour */
        }
        single_valid[mask] = ok;
    }

    /* Top & bottom validity – a 1 has only the neighbour below or above. */
    for (int mid = 0; mid < 64; ++mid) {
        for (int other = 0; other < 64; ++other) {
            bool ok_top = true, ok_bot = true;
            for (int col = 0; col < 6; ++col) {
                if (!(mid >> col & 1)) continue;
                int h = horiz_neigh[mid][col];
                int cnt = h + ((other >> col) & 1);
                if (cnt != 1 && cnt != 3 && cnt != 4) ok_top = false;
                if (cnt != 1 && cnt != 3 && cnt != 4) ok_bot = false;
                if (!ok_top && !ok_bot) break;
            }
            top_valid[mid][other]   = ok_top;
            bottom_valid[mid][other] = ok_bot;
        }
    }

    /* Middle row adjacency lists for all possible (up,row,down) triples. */
    for (int up = 0; up < 64; ++up) {
        for (int mid = 0; mid < 64; ++mid) {
            adj_t *adj = &next_adj[up * 64 + mid];
            adj->cnt = 0;
            for (int down = 0; down < 64; ++down) {
                bool ok = true;
                for (int col = 0; col < 6; ++col) {
                    if (!(mid >> col & 1)) continue;
                    int h = horiz_neigh[mid][col];
                    int cnt = h + ((up   >> col) & 1) + ((down >> col) & 1);
                    if (cnt != 1 && cnt != 3 && cnt != 4) { ok = false; break; }
                }
                if (ok) {
                    adj->list[adj->cnt++] = (uint8_t)down;
                }
            }
        }
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) precompute();

    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* n == 1 – single row case */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int mask = 0; mask < 64; ++mask)
            if (single_valid[mask]) ans += (seq_t)1;
        return ans;
    }

    /* DP tables for counts of (previous_row, current_row) pairs */
    seq_t dp[64][64];
    seq_t dp_next[64][64];

    /* initialise DP to zero */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = (seq_t)0;

    /* first two rows – check top validity */
    for (int r1 = 0; r1 < 64; ++r1)
        for (int r2 = 0; r2 < 64; ++r2)
            if (top_valid[r1][r2])
                dp[r1][r2] = (seq_t)1;

    /* n == 2 – simple bottom check */
    if (n == 2) {
        seq_t ans = (seq_t)0;
        for (int prev = 0; prev < 64; ++prev)
            for (int last = 0; last < 64; ++last)
                if (bottom_valid[last][prev])
                    ans += dp[prev][last];
        return ans;
    }

    /* n >= 3 – iterate through rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next DP */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp_next[i][j] = (seq_t)0;

        /* transition from (up,mid) to (mid,down) */
        for (int up = 0; up < 64; ++up) {
            for (int mid = 0; mid < 64; ++mid) {
                seq_t val = dp[up][mid];
                if (val == (seq_t)0) continue;
                adj_t *adj = &next_adj[up * 64 + mid];
                for (uint8_t k = 0; k < adj->cnt; ++k) {
                    uint8_t down = adj->list[k];
                    dp_next[mid][down] += val;
                }
            }
        }

        /* copy dp_next to dp for next iteration */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* final bottom check for the last row */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 64; ++prev)
        for (int last = 0; last < 64; ++last)
            if (bottom_valid[last][prev])
                ans += dp[prev][last];

    return ans;
}
