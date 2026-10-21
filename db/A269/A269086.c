/*
 * A269086  Number of nX5 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n): number of n×5 binary arrays with at most one
 * adjacent pair of 1's, where adjacency is horizontal, vertical or
 * diagonal from up‑left to down‑right.
 *
 * The DP state is:
 *   dp_prev[mask][used]  – number of ways to fill the first k rows
 *   giving previous row pattern 'mask' and whether an adjacent pair
 *   has already been seen ('used' = 0 or 1). 
 *
 * Transition:
 *   For each current row pattern 'cm', compute horizontal adjacency
 *   inside the row and vertical+down‑right adjacency with the
 *   previous row.  The total number of new pairs created is
 *   'h + nb'; this is added to the previous pair flag.
 *   If the resulting total pair count exceeds 1, the configuration
 *   is discarded.
 *
 * Complexity:  O(n·32²), trivial for n up to 10 000.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑computed tables for a single 5‑bit row */
    static unsigned horizontal[32];
    static unsigned between[32][32];
    static bool init = false;

    if (!init) {
        for (int mask = 0; mask < 32; ++mask) {
            /* Count horizontal adjacent 1‑pairs in a single row */
            horizontal[mask] = __builtin_popcount((unsigned)(mask & (mask >> 1)));
        }
        for (int p = 0; p < 32; ++p) {
            for (int c = 0; c < 32; ++c) {
                unsigned v  = __builtin_popcount((unsigned)(p & c));          /* vertical */
                unsigned d1 = __builtin_popcount((unsigned)(p & (c >> 1)));   /* down‑right diagonal */
                between[p][c] = v + d1;                                       /* no down‑left diagonal */
            }
        }
        init = true;
    }

    seq_t dp_prev[32][2];
    seq_t dp_cur[32][2];

    /* Initialise all entries to 0 */
    for (int mask = 0; mask < 32; ++mask) {
        for (int k = 0; k < 2; ++k) {
            dp_prev[mask][k] = 0;
        }
    }

    /* First row: only horizontal adjacency can occur */
    for (int mask = 0; mask < 32; ++mask) {
        unsigned h = horizontal[mask];
        if (h <= 1) {
            dp_prev[mask][h] += 1;
        }
    }

    /* Process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Clear current DP table */
        for (int mask = 0; mask < 32; ++mask) {
            for (int k = 0; k < 2; ++k) {
                dp_cur[mask][k] = 0;
            }
        }

        for (int pm = 0; pm < 32; ++pm) {
            for (int used = 0; used <= 1; ++used) {
                seq_t val = dp_prev[pm][used];
                if (val == 0) continue;
                for (int cm = 0; cm < 32; ++cm) {
                    unsigned h   = horizontal[cm];
                    unsigned nb  = between[pm][cm];
                    unsigned tot = used + h + nb;
                    if (tot <= 1) {
                        dp_cur[cm][tot] += val;
                    }
                }
            }
        }

        /* Move current DP to previous for the next iteration */
        for (int mask = 0; mask < 32; ++mask) {
            for (int k = 0; k < 2; ++k) {
                dp_prev[mask][k] = dp_cur[mask][k];
            }
        }
    }

    /* Sum all final states */
    seq_t ans = 0;
    for (int mask = 0; mask < 32; ++mask) {
        for (int k = 0; k < 2; ++k) {
            ans += dp_prev[mask][k];
        }
    }

    return ans;
}
