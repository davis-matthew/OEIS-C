/*
 * A296824  Number of n X 5 0..1 arrays with each 1 adjacent to 2, 3 or 4
 *          king-move neighboring 1's.
 * https://oeis.org/A296824
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

/* DP counting of n×5 binary arrays where each 1 is adjacent to 2–4 king‑move neighboring 1’s.
   The algorithm keeps a state consisting of the previous two rows; for each new row
   it enumerates all rows that keep the 3‑row block valid.  After processing n rows
   the last row (with a fictitious bottom row of zeros) is checked. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* pre‑computation is performed only once, even for multiple calls */
    static bool init_done = false;

    /* state encoding: 10 bits (2 rows × 5 bits each)
       index = previous_row * 32 + current_row. */
    static int state_prev[1024];
    static int state_prevprev[1024];
    static int nexts[1024][32];
    static int next_cnt[1024];
    static bool triple_valid[32][32][32];
    static seq_t dp_cur[1024];
    static seq_t dp_next[1024];

    if (!init_done) {
        /* decompose a state index into its two rows */
        for (int s = 0; s < 1024; ++s) {
            state_prevprev[s] = s / 32;
            state_prev[s]     = s % 32;
        }

        /* pre‑compute whether a triple of rows is valid */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                for (int c = 0; c < 32; ++c) {
                    bool ok = true;
                    for (int col = 0; col < 5 && ok; ++col) {
                        if ((b >> col) & 1) {
                            int cnt = 0;
                            /* above row */
                            if (a & (1 << col)) ++cnt;
                            /* bottom row */
                            if (c & (1 << col)) ++cnt;
                            /* left neighbours */
                            if (col > 0) {
                                if (a & (1 << (col - 1))) ++cnt;
                                if (b & (1 << (col - 1))) ++cnt;
                                if (c & (1 << (col - 1))) ++cnt;
                            }
                            /* right neighbours */
                            if (col < 4) {
                                if (a & (1 << (col + 1))) ++cnt;
                                if (b & (1 << (col + 1))) ++cnt;
                                if (c & (1 << (col + 1))) ++cnt;
                            }
                            /* must have 2–4 neighboring 1’s */
                            if (cnt < 2 || cnt > 4) {
                                ok = false;
                                break;
                            }
                        }
                    }
                    triple_valid[a][b][c] = ok;
                }
            }
        }

        /* pre‑compute allowed transitions */
        for (int s = 0; s < 1024; ++s) {
            int pp = state_prevprev[s];
            int p  = state_prev[s];
            int cnt = 0;
            for (int curr = 0; curr < 32; ++curr) {
                if (triple_valid[pp][p][curr]) {
                    nexts[s][cnt++] = curr;
                }
            }
            next_cnt[s] = cnt;
        }

        init_done = true;
    }

    /* DP: initialise with first row, where the fictitious preceding row is all zeros */
    for (int i = 0; i < 1024; ++i) dp_cur[i] = 0;
    for (int p = 0; p < 32; ++p) dp_cur[p] = 1;   /* state (0,p) */

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 1024; ++i) dp_next[i] = 0;
        for (int s = 0; s < 1024; ++s) {
            seq_t ways = dp_cur[s];
            if (!ways) continue;
            int cnt = next_cnt[s];
            int p   = state_prev[s];
            for (int k = 0; k < cnt; ++k) {
                int curr = nexts[s][k];
                int nxt  = p * 32 + curr;   /* new state: (prev, curr) */
                dp_next[nxt] += ways;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < 1024; ++i) dp_cur[i] = dp_next[i];
    }

    /* final check: the bottom row is all zeros */
    seq_t ans = 0;
    for (int s = 0; s < 1024; ++s) {
        if (triple_valid[state_prevprev[s]][state_prev[s]][0]) {
            ans += dp_cur[s];
        }
    }
    return ans;
}
