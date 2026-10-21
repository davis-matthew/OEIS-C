/*
 * A299217  Number of nX4 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299217: Number of n×4 binary arrays with upper-left zero
   and every element having 0,2,3,4,6 or 7 same‑value king‑move neighbours. */

#include <stdbool.h>

#define SENTRY 16          /* sentinel value for a missing row */

static bool ok_table[17][16][17];
static int ok_initialized = 0;

/* Precompute the local feasibility table for every triple of rows
   (above, middle, below). */
static void init_ok(void) {
    if (ok_initialized) return;
    ok_initialized = 1;
    for (int a = 0; a <= SENTRY; ++a) {
        for (int m = 0; m < 16; ++m) {
            for (int b = 0; b <= SENTRY; ++b) {
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    int v = (m >> col) & 1;
                    int cnt = 0;
                    for (int dr = -1; dr <= 1; ++dr) {
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue;   /* skip self */
                            int ncol = col + dc;
                            if (ncol < 0 || ncol >= 4) continue;  /* outside columns */
                            int nb;
                            if (dr == -1) {
                                if (a == SENTRY) continue;        /* no above neighbour */
                                nb = (a >> ncol) & 1;
                            } else if (dr == 0) {
                                nb = (m >> ncol) & 1;
                            } else { /* dr == 1 */
                                if (b == SENTRY) continue;        /* no below neighbour */
                                nb = (b >> ncol) & 1;
                            }
                            if (nb == v) ++cnt;
                        }
                    }
                    if (!(cnt == 0 || cnt == 2 || cnt == 3 ||
                          cnt == 4 || cnt == 6 || cnt == 7)) {
                        ok = false;
                        break;
                    }
                }
                ok_table[a][m][b] = ok;
            }
        }
    }
}

seq_t a(idx_t n) {
    init_ok();
    if (n == 0) return 0;
    idx_t rows = n;

    /* dp_cur[above][current] holds the number of ways after having processed
       the last two rows: "above" is the row before the last, "current" is
       the last row that is already validated.  */
    seq_t dp_cur[17][16] = { {0} };
    seq_t dp_next[17][16] = { {0} };

    /* Initialise with the first row.  The upper‑left element must be 0. */
    for (int r = 0; r < 16; ++r) {
        if ((r & 1) == 0)              /* bit 0 (leftmost column) == 0 */
            dp_cur[SENTRY][r] = 1;
    }

    /* Process rows 1 .. rows-1 */
    for (idx_t i = 1; i < rows; ++i) {
        /* Zero the next‑layer table */
        for (int a = 0; a <= SENTRY; ++a)
            for (int m = 0; m < 16; ++m)
                dp_next[a][m] = 0;

        for (int prev2 = 0; prev2 <= SENTRY; ++prev2) {
            for (int prev1 = 0; prev1 < 16; ++prev1) {
                seq_t ways = dp_cur[prev2][prev1];
                if (ways == 0) continue;
                for (int newRow = 0; newRow < 16; ++newRow) {
                    if (ok_table[prev2][prev1][newRow]) {
                        dp_next[prev1][newRow] += ways;
                    }
                }
            }
        }
        /* Swap layers */
        for (int a = 0; a <= SENTRY; ++a)
            for (int m = 0; m < 16; ++m)
                dp_cur[a][m] = dp_next[a][m];
    }

    /* Finalisation: validate the last row (row n-1) using the sentinel below */
    seq_t ans = 0;
    for (int prev = 0; prev <= SENTRY; ++prev) {
        for (int last = 0; last < 16; ++last) {
            seq_t ways = dp_cur[prev][last];
            if (ways == 0) continue;
            if (ok_table[prev][last][SENTRY]) {
                ans += ways;
            }
        }
    }
    return ans;
}
