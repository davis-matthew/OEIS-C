/*
 * A254848  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum one and no antidiagonal sum two.
 * https://oeis.org/A254848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary matrices with no 3×3 sub‑block
 *            whose main diagonal sums to 1 and no 3×3 sub‑block whose
 *            anti‑diagonal sums to 2. Computed by dynamic programming over
 *            successive rows
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 43, u128 n <= 93,
 *            big any n
 * Helpers:   bit_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_val.h"

/* compile‑time constants for the DP */
enum {
    ROW_BITS   = 4,                    /* width of each row   */
    ROW_WIDTH  = 1 << ROW_BITS,        /* 16 different rows   */
    STATE_COUNT = ROW_WIDTH * ROW_WIDTH,  /* 256 possible last‑two‑row states */
    MAX_TRANS  = 16                    /* at most 16 successors per state */
};

static unsigned char next_cnt[STATE_COUNT];
static int next_state[STATE_COUNT][MAX_TRANS];
static bool init_done = false;

/* test whether a transition a->b->c is allowed (i.e. all 3×3 blocks
   containing rows a,b,c satisfy the constraints) */
static inline bool allowed_transition(int a, int b, int c) {
    for (int j = 0; j < 2; ++j) {
        int diag  = bit_val(a, j)     + bit_val(b, j + 1) + bit_val(c, j + 2);
        if (diag == 1) return false;
        int anti = bit_val(a, j + 2) + bit_val(b, j + 1) + bit_val(c, j);
        if (anti == 2) return false;
    }
    return true;
}

/* build transition tables once */
static void init_transitions(void) {
    for (int a = 0; a < ROW_WIDTH; ++a) {
        for (int b = 0; b < ROW_WIDTH; ++b) {
            int s = (a << ROW_BITS) | b;
            next_cnt[s] = 0;
            for (int c = 0; c < ROW_WIDTH; ++c) {
                if (allowed_transition(a, b, c)) {
                    int t = (b << ROW_BITS) | c;
                    next_state[s][next_cnt[s]++] = t;
                }
            }
        }
    }
    init_done = true;
}

/* main sequence function ------------------------------------------- */
seq_t a(idx_t n) {
    if (!init_done) init_transitions();

    seq_t dp[STATE_COUNT];
    seq_t ndp[STATE_COUNT];

    /* initial state: any pair of first two rows is allowed */
    for (int s = 0; s < STATE_COUNT; ++s) dp[s] = (seq_t)1;

    /* each step adds one new row; we need n such steps (total rows n+2) */
    for (idx_t step = 0; step < n; ++step) {
        for (int t = 0; t < STATE_COUNT; ++t) ndp[t] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            unsigned char m = next_cnt[s];
            for (int k = 0; k < m; ++k) {
                int t = next_state[s][k];
                ndp[t] += val;
            }
        }

        /* copy new counts back to dp */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = ndp[i];
    }

    /* sum over all final states */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) total += dp[s];
    return total;
}
