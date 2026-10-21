/*
 * A283039  Number of n X 5 0..1 arrays with no 1 equal to more than one of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly one element.
 * https://oeis.org/A283039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283039: Number of n×5 0..1 arrays with at most one cell having
   more than one horizontal or vertical neighbour.  The algorithm
   is a transfer‑matrix DP over the rows.
   (See OEIS entry for wording and sample solutions.) */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAXP 32          /* 2^5 patterns for a row */
#define COLS 5

/* pre‑computed tables: whether a transition is valid and whether it
   introduces a violation in the middle row.  They are filled once
   on first entry to a(n). */
static bool trans_valid[MAXP][MAXP][MAXP];
static bool trans_violate[MAXP][MAXP][MAXP];
static bool tables_ready = false;

/* DP tables: dp[prevprev][prev][exception_used] */
static seq_t dp[MAXP][MAXP][2];
static seq_t nxt[MAXP][MAXP][2];

seq_t a(idx_t n)
{
    /* Build transition tables on first call. */
    if (!tables_ready) {
        for (int pp = 0; pp < MAXP; ++pp) {
            for (int p  = 0; p  < MAXP; ++p) {
                for (int c  = 0; c  < MAXP; ++c) {
                    int violations = 0;
                    for (int j = 0; j < COLS; ++j) {
                        if (!((p >> j) & 1)) continue;
                        int deg = 0;
                        if (j > 0     && ((p >> (j-1)) & 1)) ++deg; /* left */
                        if (j + 1 < COLS && ((p >> (j+1)) & 1)) ++deg; /* right */
                        if ((pp >> j) & 1) ++deg;                    /* up */
                        if ((c  >> j) & 1) ++deg;                    /* down */
                        if (deg > 1) {
                            ++violations;
                            if (violations > 1) break;
                        }
                    }
                    if (violations > 1) {
                        trans_valid[pp][p][c] = false;
                        trans_violate[pp][p][c] = false;
                    } else {
                        trans_valid[pp][p][c] = true;
                        trans_violate[pp][p][c] = (violations == 1);
                    }
                }
            }
        }
        tables_ready = true;
    }

    /* initialise DP */
    for (int pp = 0; pp < MAXP; ++pp)
        for (int p = 0; p < MAXP; ++p)
            for (int e = 0; e < 2; ++e)
                dp[pp][p][e] = 0;
    dp[0][0][0] = 1;           /* dummy row before grid */

    /* process each row */
    for (idx_t row = 1; row <= n; ++row) {
        /* clear next table */
        for (int pp = 0; pp < MAXP; ++pp)
            for (int p = 0; p < MAXP; ++p)
                for (int e = 0; e < 2; ++e)
                    nxt[pp][p][e] = 0;

        for (int pp = 0; pp < MAXP; ++pp) {
            for (int p = 0; p < MAXP; ++p) {
                for (int e = 0; e < 2; ++e) {
                    seq_t ways = dp[pp][p][e];
                    if (ways == 0) continue;
                    for (int c = 0; c < MAXP; ++c) {
                        if (!trans_valid[pp][p][c]) continue;
                        int new_e = e;
                        if (trans_violate[pp][p][c]) {
                            if (e) continue;      /* second violation */
                            new_e = 1;
                        }
                        nxt[p][c][new_e] += ways;
                    }
                }
            }
        }
        /* advance to next row */
        for (int pp = 0; pp < MAXP; ++pp)
            for (int p = 0; p < MAXP; ++p)
                for (int e = 0; e < 2; ++e)
                    dp[pp][p][e] = nxt[pp][p][e];
    }

    /* finalisation: last row has no down neighbour */
    seq_t ans = 0;
    for (int pp = 0; pp < MAXP; ++pp) {
        for (int p = 0; p < MAXP; ++p) {
            for (int e = 0; e < 2; ++e) {
                seq_t ways = dp[pp][p][e];
                if (ways == 0) continue;
                int violations = 0;
                for (int j = 0; j < COLS; ++j) {
                    if (!((p >> j) & 1)) continue;
                    int deg = 0;
                    if (j > 0     && ((p >> (j-1)) & 1)) ++deg; /* left */
                    if (j + 1 < COLS && ((p >> (j+1)) & 1)) ++deg; /* right */
                    if ((pp >> j) & 1) ++deg;                    /* up */
                    /* no down neighbour */
                    if (deg > 1) {
                        ++violations;
                        if (violations > 1) break;
                    }
                }
                if (violations > 1) continue;          /* too many violations */

                int final_e = e;
                if (violations == 1) {
                    if (e) continue;                  /* second violation */
                    final_e = 1;
                }
                if (final_e != 1) continue;           /* need exactly one exception */
                ans += ways;
            }
        }
    }
    return ans;
}
