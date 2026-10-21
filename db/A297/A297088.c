/*
 * A297088  Number of n X 2 0..1 arrays with no 1 adjacent to 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A297088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts n×2 binary arrays with no 1 having exactly three
   king‑move neighbors that are 1.  
   We use a 3‑row sliding window DP: for each triple (prev, curr, next)
   we pre‑compute whether the middle row curr satisfies the constraint
   given its two adjacent rows.  Then we propagate counts, padding the
   grid with an all‑zero sentinel row before the first and after the last
   real row. */
#include <stdbool.h>

static bool valid[4][4][4];

/* Precompute the validity table for all triples of 2‑bit rows. */
static void init_valid(void) {
    for (int prev = 0; prev < 4; ++prev) {
        for (int curr = 0; curr < 4; ++curr) {
            for (int next = 0; next < 4; ++next) {
                bool ok = true;
                for (int j = 0; j < 2; ++j) {
                    int bit = (curr >> j) & 1;
                    if (bit == 0) continue;
                    int cnt = 0;
                    /* up */
                    cnt += (prev >> j) & 1;
                    /* down */
                    cnt += (next >> j) & 1;
                    /* horizontal */
                    cnt += (curr >> (1 - j)) & 1;
                    /* diagonal up-left/right */
                    cnt += (prev >> (1 - j)) & 1;
                    /* diagonal down-left/right */
                    cnt += (next >> (1 - j)) & 1;
                    if (cnt == 3) {
                        ok = false;
                        break;
                    }
                }
                valid[prev][curr][next] = ok;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    static bool inited = false;
    if (!inited) {
        init_valid();
        inited = true;
    }

    /* DP arrays: dp[prev][curr] holds number of ways up to current row. */
    seq_t dp_curr[4][4] = {0};
    seq_t dp_next[4][4] = {0};

    /* Row 0 is the all‑zero sentinel */
    for (int curr = 0; curr < 4; ++curr)
        dp_curr[0][curr] = (seq_t)1;

    /* Process rows 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b)
                dp_next[a][b] = (seq_t)0;

        for (int prev = 0; prev < 4; ++prev) {
            for (int curr = 0; curr < 4; ++curr) {
                seq_t val = dp_curr[prev][curr];
                if (val == (seq_t)0) continue;
                for (int nxt = 0; nxt < 4; ++nxt) {
                    if (!valid[prev][curr][nxt]) continue;
                    dp_next[curr][nxt] += val;
                }
            }
        }

        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b)
                dp_curr[a][b] = dp_next[a][b];
    }

    /* Final validation: row n with next sentinel row 0 */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 4; ++prev) {
        for (int curr = 0; curr < 4; ++curr) {
            if (!valid[prev][curr][0]) continue;
            ans += dp_curr[prev][curr];
        }
    }

    return ans;
}
