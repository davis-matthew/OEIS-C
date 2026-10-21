/*
 * A267731  Number of nX4 0..1 arrays with every repeated value in every row
 *          and column greater than or equal to the previous repeated value.
 * https://oeis.org/A267731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267731:
   DP over rows with column state (0 = no repeat yet,
   1 = seen repeated 0, 2 = seen repeated 1).
   Transition: when two consecutive rows have equal bit in a column,
   that column state is updated; a new repeated 0 after state 2 is
   forbidden.
   The function returns a(n) for n ≥ 1.   */

#include <stdbool.h>

#define STATE_COLS 81   /* 3^4 */

static const int bit_of[16][4] = {
    {0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1},
    {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1},
    {1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1},
    {1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}
};

static const bool valid_row[16] = {
    true, true, true, true, true, true, true, true,
    true, true, true, true, false, true, true, true
};

/* Predecode 3‑base state number into 4 digits (each 0..2). */
static int col_state_dec[STATE_COLS][4];

static void precompute_decoding(void)
{
    for (int s = 0; s < STATE_COLS; ++s) {
        int tmp = s;
        for (int c = 3; c >= 0; --c) {
            col_state_dec[s][c] = tmp % 3;
            tmp /= 3;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) not defined, return 0 */
        return (seq_t)0;

    /* DP arrays: 16 patterns × 81 column states */
    static seq_t dp[16 * STATE_COLS];
    static seq_t dp_next[16 * STATE_COLS];
    const int TOTAL = 16 * STATE_COLS;

    /* clear dp */
    for (int i = 0; i < TOTAL; ++i)
        dp[i] = (seq_t)0;

    /* first row: no column repeats yet, state = 0 for all columns */
    for (int pat = 0; pat < 16; ++pat) {
        if (!valid_row[pat]) continue;
        dp[pat * STATE_COLS] = (seq_t)1;
    }

    /* predecode once */
    static bool decoded = false;
    if (!decoded) {
        precompute_decoding();
        decoded = true;
    }

    /* process subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear dp_next */
        for (int i = 0; i < TOTAL; ++i)
            dp_next[i] = (seq_t)0;

        /* iterate over all existing states */
        for (int prev_pat = 0; prev_pat < 16; ++prev_pat) {
            for (int col_state = 0; col_state < STATE_COLS; ++col_state) {
                seq_t val = dp[prev_pat * STATE_COLS + col_state];
                if (val == (seq_t)0) continue;

                /* decode column states */
                int st[4];
                st[0] = col_state_dec[col_state][0];
                st[1] = col_state_dec[col_state][1];
                st[2] = col_state_dec[col_state][2];
                st[3] = col_state_dec[col_state][3];

                for (int pat = 0; pat < 16; ++pat) {
                    if (!valid_row[pat]) continue;

                    int new_state[4];
                    bool bad = false;

                    /* column 0 */
                    if (bit_of[prev_pat][0] == bit_of[pat][0]) {
                        if (bit_of[pat][0] == 0) {
                            if (st[0] == 2) { bad = true; }
                            else            new_state[0] = 1;
                        } else {
                            new_state[0] = 2;
                        }
                    } else {
                        new_state[0] = st[0];
                    }

                    /* column 1 */
                    if (!bad && bit_of[prev_pat][1] == bit_of[pat][1]) {
                        if (bit_of[pat][1] == 0) {
                            if (st[1] == 2) { bad = true; }
                            else            new_state[1] = 1;
                        } else {
                            new_state[1] = 2;
                        }
                    } else if (!bad) {
                        new_state[1] = st[1];
                    }

                    /* column 2 */
                    if (!bad && bit_of[prev_pat][2] == bit_of[pat][2]) {
                        if (bit_of[pat][2] == 0) {
                            if (st[2] == 2) { bad = true; }
                            else            new_state[2] = 1;
                        } else {
                            new_state[2] = 2;
                        }
                    } else if (!bad) {
                        new_state[2] = st[2];
                    }

                    /* column 3 */
                    if (!bad && bit_of[prev_pat][3] == bit_of[pat][3]) {
                        if (bit_of[pat][3] == 0) {
                            if (st[3] == 2) { bad = true; }
                            else            new_state[3] = 1;
                        } else {
                            new_state[3] = 2;
                        }
                    } else if (!bad) {
                        new_state[3] = st[3];
                    }

                    if (bad) continue;

                    /* encode new column state */
                    int ns = ((new_state[0] * 3 + new_state[1]) * 3 + new_state[2]) * 3 + new_state[3];
                    int idx = pat * STATE_COLS + ns;
                    dp_next[idx] = dp_next[idx] + val;
                }
            }
        }

        /* swap dp arrays */
        for (int i = 0; i < TOTAL; ++i)
            dp[i] = dp_next[i];
    }

    /* sum all remaining states to get the answer */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < TOTAL; ++i)
        ans = ans + dp[i];

    return ans;
}
