/*
 * A269074  Number of nX7 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269074
   Number of n×7 binary arrays with at most one pair of adjacent 1s
   horizontally, diagonally or antidiagonally.
   Implements a transfer‑matrix DP scanning rows.
   a(1) = 72, a(2) = 1204, ... (see OEIS for further terms).
*/

#include <stdint.h>
#include <stdbool.h>

/* Tables for horizontal and cross (vertical/diagonal) adjacency counts. */
static bool init_done = false;
static int8_t  hor_pairs[1 << 7];
static uint8_t cross_pairs[1 << 7][1 << 7];

static void init_tables(void)
{
    if (init_done) return;

    const int W = 7;
    const int L = 1 << W;

    for (int p = 0; p < L; ++p) {
        int h = 0;
        for (int i = 0; i < W-1; ++i) {
            if ( (p>>i)&1 && (p>>(i+1))&1 ) ++h;
        }
        hor_pairs[p] = (int8_t)h;
    }

    for (int p = 0; p < L; ++p) {
        for (int q = 0; q < L; ++q) {
            int d = 0;
            /* down‑right diagonal */
            for (int i = 0; i < W-1; ++i)
                if ( (p>>i)&1 && (q>>(i+1))&1 ) ++d;
            /* down‑left diagonal */
            for (int i = 1; i < W; ++i)
                if ( (p>>i)&1 && (q>>(i-1))&1 ) ++d;
            cross_pairs[p][q] = (uint8_t)d;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;     /* offset starts at 1 */

    const int L = 1 << 7;
    seq_t dp[L][2];
    seq_t newdp[L][2];

    /* initialise dp to zero */
    for (int i = 0; i < L; ++i) dp[i][0] = dp[i][1] = 0;

    /* first row */
    for (int p = 0; p < L; ++p) {
        int h = hor_pairs[p];
        if (h == 0) dp[p][0] = 1;
        else if (h == 1) dp[p][1] = 1;
    }

    /* remaining rows */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < L; ++i) newdp[i][0] = newdp[i][1] = 0;

        for (int prev = 0; prev < L; ++prev) {
            for (int used = 0; used < 2; ++used) {
                seq_t cur = dp[prev][used];
                if (!cur) continue;

                for (int curp = 0; curp < L; ++curp) {
                    int pairs = hor_pairs[curp] + cross_pairs[prev][curp];
                    if (used == 0) {
                        if (pairs == 0)           newdp[curp][0] += cur;
                        else if (pairs == 1)      newdp[curp][1] += cur;
                    } else { /* used == 1 */
                        if (pairs == 0)           newdp[curp][1] += cur;
                    }
                }
            }
        }
        /* copy newdp → dp */
        for (int i = 0; i < L; ++i) {
            dp[i][0] = newdp[i][0];
            dp[i][1] = newdp[i][1];
        }
    }

    /* sum all valid end states */
    seq_t result = 0;
    for (int p = 0; p < L; ++p) {
        result += dp[p][0];
        result += dp[p][1];
    }
    return result;
}
