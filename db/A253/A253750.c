/*
 * A253750  Number of (2+1)X(n+1) 0..2 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A253750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (2+1)×(n+1) 0..2 arrays with horizontal *
 *            nondecreasing antidiagonal differences and vertical
 *            nondecreasing * nw+se diagonal sums. * * Computation: dynamic
 *            programming over column pairs. Each column has * three entries
 *            0,1,2 and the state consists of the last column plus the * pr...
 * Range:     u8 none, u16 n <= 4, u32 n <= 22, u64 n <= 57, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define VALS   3
#define NUM_COLS (VALS*VALS*VALS)   /* 3^3 = 27 */
#define DIFF_RANGE 5                /* -2 .. +2 */
#define STATE_BITS (NUM_COLS * DIFF_RANGE * DIFF_RANGE)  /* 27*5*5 = 675 */
static int trans_next[STATE_BITS][ VALS*VALS*VALS ];      /* up to 27 transitions per state */
static unsigned char trans_count[STATE_BITS];
static bool trans_built = false;

static void build_transitions(void)
{
    if (trans_built) return;
    static int col0[NUM_COLS], col1[NUM_COLS], col2[NUM_COLS];
    for (int i = 0; i < NUM_COLS; ++i) {
        col0[i] = i / 9;
        col1[i] = (i % 9) / 3;
        col2[i] = i % 3;
    }
    for (int i = 0; i < STATE_BITS; ++i) trans_count[i] = 0;
    for (int s = 0; s < STATE_BITS; ++s) {
        int col_idx = s / DIFF_RANGE / DIFF_RANGE;   /* s = col*25 + rem, col = s/25 */
        int rem = s % DIFF_RANGE / DIFF_RANGE;       /* ?? not used actually */
        /* easier: col_idx = s / 25; */
        col_idx = s / 25;
        int rem2 = s % 25;
        int d0prev = (rem2 / 5) - 2;
        int d1prev = (rem2 % 5) - 2;
        int a0 = col0[col_idx];
        int a1 = col1[col_idx];
        int a2 = col2[col_idx];
        for (int b0 = 0; b0 < VALS; ++b0)
            for (int b1 = 0; b1 < VALS; ++b1)
                for (int b2 = 0; b2 < VALS; ++b2) {
                    int d0new = b0 - a1;
                    if (d0new < d0prev) continue;
                    int d1new = b1 - a2;
                    if (d1new < d1prev) continue;
                    if (a0 + b1 > a1 + b2) continue;
                    int new_col = b0 * 9 + b1 * 3 + b2;
                    int new_state = new_col * 25 + (d0new + 2) * 5 + (d1new + 2);
                    trans_next[s][trans_count[s]++] = new_state;
                }
    }
    trans_built = true;
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t dp_cur[STATE_BITS];
    static seq_t dp_next[STATE_BITS];
    static seq_t *seq_vals = NULL;
    static size_t seq_cap = 0;
    static size_t seq_used = 0;
    static size_t computed_steps = 0;   /* steps already performed = current n */

    if (!init) {
        build_transitions();
        /* init dp_cur to 1 for all columns with previous diffs = -2 */
        for (int i = 0; i < STATE_BITS; ++i) dp_cur[i] = 0;
        for (int col = 0; col < NUM_COLS; ++col) {
            int state = col * 25 + 0 * 5 + 0;  /* d0=d1=-2 --> indices 0,0 */
            dp_cur[state] = 1;
        }
        /* allocate seq_vals */
        seq_cap = 64;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_vals[0] = 0;   /* dummy for n=0 */
        seq_used = 1;
        computed_steps = 0;
        init = true;
    }

    /* make sure we have room for requested n */
    if (n >= seq_cap) {
        size_t new_cap = seq_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        for (size_t i = seq_cap; i < new_cap; ++i) seq_vals[i] = 0;
        seq_cap = new_cap;
    }

    /* advance DP until we reach desired n */
    while (computed_steps < n) {
        for (int i = 0; i < STATE_BITS; ++i) dp_next[i] = 0;
        for (int s = 0; s < STATE_BITS; ++s) {
            if (dp_cur[s] == 0) continue;
            seq_t val = dp_cur[s];
            unsigned char cnt = trans_count[s];
            for (unsigned char t = 0; t < cnt; ++t) {
                int ns = trans_next[s][t];
                dp_next[ns] += val;
            }
        }
        /* sum all states to get a(n) */
        seq_t sum = 0;
        for (int i = 0; i < STATE_BITS; ++i) sum += dp_next[i];
        ++computed_steps;
        seq_vals[computed_steps] = sum;
        /* copy next to current */
        for (int i = 0; i < STATE_BITS; ++i) dp_cur[i] = dp_next[i];
    }

    return seq_vals[n];
}
