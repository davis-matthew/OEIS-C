/*
 * A275186  Number of 6 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A275186 – counts 6 × n 0..2 arrays with forbidden offsets * and
 *            with new values introduced in order 0,1,2. The function uses a *
 *            transfer‑matrix DP derived from the constraints, which
 *            reproduces all * terms of the OEIS sequence
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Number of admissible 6‑row states in a single column.
 * 3^6 = 729 possible rows, 96 of them satisfy the vertical adjacency
 * and “new value order” constraint. */
#define STATE_COUNT 96

/* Tables describing the 96 admissible column states. */
static int  g_state_rows[STATE_COUNT][6];
static bool g_contains1[STATE_COUNT];
static int  g_first1[STATE_COUNT], g_first2[STATE_COUNT];
static bool g_valid[STATE_COUNT];

static bool g_crossValid[STATE_COUNT][STATE_COUNT];
static bool g_allow0[STATE_COUNT][STATE_COUNT];
static int  g_nextCount[STATE_COUNT];
static int  g_nextIdx[STATE_COUNT][STATE_COUNT];

/* Pre‑compute the tables once. */
static bool g_initialized = false;
static void init_tables(void) {
    if (g_initialized) return;
    int s = 0;
    /* Enumerate all 3^6 patterns. */
    for (int num = 0; num < 729; ++num) {
        int v = num;
        bool ok = true;
        int prev = -1;
        int vals[6];
        for (int r = 0; r < 6; ++r) {
            int cur = v % 3;
            v /= 3;
            /* vertical adjacency forbids equal neighbours in the same column */
            if (prev == cur) { ok = false; break; }
            vals[r] = cur;
            prev = cur;
        }
        if (!ok) continue;

        /* Store the state. */
        for (int r = 0; r < 6; ++r) g_state_rows[s][r] = vals[r];

        /* Pre‑compute characteristics. */
        bool has1 = false, has2 = false;
        int f1 = 6, f2 = 6;
        for (int r = 0; r < 6; ++r) {
            if (vals[r] == 1) { has1 = true; if (f1 == 6) f1 = r; }
            if (vals[r] == 2) { has2 = true; if (f2 == 6) f2 = r; }
        }
        g_contains1[s] = has1;
        g_first1[s]   = f1;
        g_first2[s]   = f2;

        /* Internal validity: a 2 may only appear after a 1, so
         * if the state contains a 2, it must also contain a 1 and
         * the first 2 must come after the first 1. */
        if (f2 == 6) g_valid[s] = true;
        else if (f1 == 6 || f2 <= f1) g_valid[s] = false;
        else g_valid[s] = true;

        ++s;
    }

    /* Compute transitions between admissible states. */
    for (int p = 0; p < s; ++p) {
        g_nextCount[p] = 0;
        for (int c = 0; c < s; ++c) {
            bool cross = true;
            for (int pr = 0; pr < 6; ++pr) {
                /* edge (pr) in prev -> (pr-1) in cur */
                if (pr - 1 >= 0) {
                    int cr = g_state_rows[c][pr - 1];
                    if (cr == g_state_rows[p][pr]) { cross = false; break; }
                }
                /* edge (pr) in prev -> (pr+2) in cur */
                if (pr + 2 <= 5) {
                    int cr = g_state_rows[c][pr + 2];
                    if (cr == g_state_rows[p][pr]) { cross = false; break; }
                }
            }
            g_crossValid[p][c] = cross;
            if (!cross) continue;

            g_nextIdx[p][g_nextCount[p]++] = c;

            /* Transition allowed when we have not seen a 1 yet in previous columns. */
            bool allow0 = true;
            if (g_first2[c] != 6) {          /* state c contains a 2 */
                if (g_first1[c] == 6 || g_first2[c] <= g_first1[c])
                    allow0 = false;          /* 2 appears before any 1 -> forbidden */
            }
            g_allow0[p][c] = allow0;
        }
    }
    g_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_tables();
    if (n == 0) return 0;   /* offset is 1 – safety */

    /* dp[state][seen1_flag] – number of ways to reach this state after processing a column. */
    seq_t dp[STATE_COUNT][2];
    seq_t ndp[STATE_COUNT][2];
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp[i][0] = dp[i][1] = 0;
        ndp[i][0] = ndp[i][1] = 0;
    }

    /* Initialise first column: top row must be 0 and the state must be internally valid. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (g_state_rows[s][0] != 0) continue;    /* top must be 0 */
        if (!g_valid[s]) continue;                /* internal ordering of 1,2 */
        int flag = g_contains1[s] ? 1 : 0;        /* whether a 1 has appeared in this column */
        dp[s][flag] = 1;
    }

    /* Process remaining columns. */
    for (idx_t col = 2; col <= n; ++col) {
        /* reset ndp */
        for (int i = 0; i < STATE_COUNT; ++i) {
            ndp[i][0] = ndp[i][1] = 0;
        }
        for (int p = 0; p < STATE_COUNT; ++p) {
            for (int f = 0; f <= 1; ++f) {
                seq_t val = dp[p][f];
                if (val == 0) continue;
                int nextcnt = g_nextCount[p];
                for (int t = 0; t < nextcnt; ++t) {
                    int c = g_nextIdx[p][t];
                    if (f == 0 && !g_allow0[p][c]) continue;    /* ordering violation */
                    int nf = f | (g_contains1[c] ? 1 : 0);
                    ndp[c][nf] += val;
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) {
            dp[i][0] = ndp[i][0];
            dp[i][1] = ndp[i][1];
        }
    }

    /* All valid arrays must have seen at least one 1 somewhere. */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) result += dp[s][1];
    return result;
}
