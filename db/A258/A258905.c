/*
 * A258905  Number of (n+2)X(3+2) 0..1 arrays with every 3X3 subblock diagonal
 *          median plus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A258905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over rows and columns */

/* Required headers for types */
#include <stdint.h>
#include <stdbool.h>

/*
 *  Helper: mapping from S = (s0,s1,s2) to an index 0..9
 */
static int s_map[3][3][3];
static int s_val[10][3];

/* Precompute S‑index table */
static void init_s_map(void) {
    int idx = 0;
    for (int s0 = 0; s0 < 3; ++s0)
        for (int s1 = s0; s1 < 3; ++s1)
            for (int s2 = s1; s2 < 3; ++s2) {
                s_map[s0][s1][s2] = idx;
                s_val[idx][0] = s0;
                s_val[idx][1] = s1;
                s_val[idx][2] = s2;
                ++idx;
            }
}

/* Table: triple_s[a][b][c] gives S‑index for rows a,b,c
   or -1 if horizontal monotonicity fails. */
static int8_t triple_s[32][32][32];

/* Precompute triple_s */
static void init_triple_s(void) {
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            for (int c = 0; c < 32; ++c) {
                int s[3];
                for (int j = 0; j < 3; ++j) {
                    int diag = ((a >> j) & 1)
                             + ((b >> (j + 1)) & 1)
                             + ((c >> (j + 2)) & 1);
                    int anti = ((a >> (j + 2)) & 1)
                             + ((b >> (j + 1)) & 1)
                             + ((c >> j) & 1);
                    s[j] = (diag >= 2) + (anti >= 2);
                }
                if (s[0] <= s[1] && s[1] <= s[2])
                    triple_s[a][b][c] = (int8_t)s_map[s[0]][s[1]][s[2]];
                else
                    triple_s[a][b][c] = -1;
            }
}

/* Transition table: for each pair (a,b) we store all (c, sidx)
   such that triple_s[a][b][c] != -1. */
typedef struct { uint8_t next; uint8_t sidx; } Trans;
static Trans trans[32][32][32];
static uint8_t trans_cnt[32][32];

/* Precompute transitions */
static void init_trans(void) {
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b) {
            uint8_t cnt = 0;
            for (int c = 0; c < 32; ++c) {
                int8_t idx = triple_s[a][b][c];
                if (idx >= 0) {
                    trans[a][b][cnt].next = (uint8_t)c;
                    trans[a][b][cnt].sidx = (uint8_t)idx;
                    ++cnt;
                }
            }
            trans_cnt[a][b] = cnt;
        }
}

/* DP arrays */
static seq_t dp[32][32][10];
static seq_t dp_next[32][32][10];

/* Main function */
seq_t a(idx_t n) {
    /* One-time init guard */
    static bool init = false;
    if (!init) {
        init_s_map();
        init_triple_s();
        init_trans();
        init = true;
    }

    /* Total rows = n+2 */
    idx_t N = n + 2;

    /* Clear dp */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            for (int s = 0; s < 10; ++s)
                dp[a][b][s] = 0;

    /* Base: process first three rows */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            for (int c = 0; c < 32; ++c) {
                int8_t idx = triple_s[a][b][c];
                if (idx >= 0)
                    dp[b][c][idx] += 1;
            }

    /* DP for remaining rows (from index 3 to N-1) */
    for (idx_t step = 3; step < N; ++step) {
        /* clear dp_next */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                for (int s = 0; s < 10; ++s)
                    dp_next[a][b][s] = 0;

        /* transition */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                for (int sprev = 0; sprev < 10; ++sprev) {
                    seq_t val = dp[a][b][sprev];
                    if (val == 0) continue;
                    for (uint8_t t = 0; t < trans_cnt[a][b]; ++t) {
                        uint8_t nextrow = trans[a][b][t].next;
                        uint8_t sidx   = trans[a][b][t].sidx;
                        /* vertical inequalities */
                        if (s_val[sprev][0] > s_val[sidx][0]) continue;
                        if (s_val[sprev][1] > s_val[sidx][1]) continue;
                        if (s_val[sprev][2] > s_val[sidx][2]) continue;
                        dp_next[b][nextrow][sidx] += val;
                    }
                }
            }
        }

        /* swap dp and dp_next */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                for (int s = 0; s < 10; ++s)
                    dp[a][b][s] = dp_next[a][b][s];
    }

    /* Sum all counts: the answer */
    seq_t ans = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            for (int s = 0; s < 10; ++s)
                ans += dp[a][b][s];
    return ans;
}
