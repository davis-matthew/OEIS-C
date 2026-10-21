/*
 * A295412  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 2 or 3 1s.
 * https://oeis.org/A295412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A295412 – Number of n×4 binary arrays in which each 1 has 1–3
 * horizontally or vertically adjacent 1s.  The implementation uses
 * a transfer‑matrix dynamic programming for width 4.  For each column
 * the state stores whether the cell is 0 or 1 together with the
 * number of neighbours already seen (up, left, right).  A state is
 * represented in base 5: digit 0 means the cell is 0; digits 1–4
 * mean the cell is 1 with count 0…3.  There are 5⁴ = 625 states.
 * Transitions are pre‑computed once.  For any n≥1 the function
 * memoises results so that calls may occur in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants ---------------------------------------------------- */
#define WIDTH      4          /* number of columns */
#define STATE_CNT  625        /* 5⁴ possible states */
#define TRANS_CNT  16         /* number of row patterns (4 bits) */

/* Tables – initialised on first call -------------------------------- */
static uint8_t state_digits[STATE_CNT][WIDTH];   /* digits of a state    */
static uint8_t row_bits[TRANS_CNT][WIDTH];       /* bits for each row    */
static int16_t trans_state[STATE_CNT][TRANS_CNT];/* transition table    */
static bool    last_valid[STATE_CNT];            /* valid final column   */
static const int pow5[WIDTH] = {1, 5, 25, 125};

static bool initialized = false;

/* --------------------------------------------------------------- */
static void init_tables(void)
{
    if (initialized)
        return;

    /* State digits: 0 → cell 0
     *                1–4 → cell 1 with count = digit‑1 (0…3)                 */
    for (int s = 0; s < STATE_CNT; ++s) {
        int v = s;
        for (int c = 0; c < WIDTH; ++c) {
            state_digits[s][c] = (uint8_t)(v % 5);
            v /= 5;
        }
    }

    /* Row patterns (0–15) */
    for (int p = 0; p < TRANS_CNT; ++p) {
        for (int c = 0; c < WIDTH; ++c)
            row_bits[p][c] = (uint8_t)((p >> c) & 1);
    }

    /* Pre‑compute transitions and final‑row validity */
    for (int s = 0; s < STATE_CNT; ++s) {
        /* Check if s is a valid final row */
        bool ok_final = true;
        for (int c = 0; c < WIDTH; ++c) {
            if (state_digits[s][c] == 1) {   /* 1 with 0 neighbours */
                ok_final = false;
                break;
            }
        }
        last_valid[s] = ok_final;

        /* Transitions */
        for (int p = 0; p < TRANS_CNT; ++p) {
            bool ok = true;

            /* 1) Verify that each 1 in the previous row will have
             *    between 1 and 3 neighbours after adding the down
             *    neighbour from the current row.                         */
            for (int c = 0; c < WIDTH && ok; ++c) {
                bool prev_bit = state_digits[s][c] != 0;
                if (!prev_bit)
                    continue;
                int prev_cnt = state_digits[s][c] - 1;   /* already seen */
                int down_bit = row_bits[p][c];
                int total = prev_cnt + down_bit;
                if (total == 0 || total > 3)
                    ok = false;
            }

            /* 2) Compute the new state for the current row */
            int new_state = 0;   /* correct initialisation */
            if (ok) {
                uint8_t newdig[WIDTH];
                for (int c = 0; c < WIDTH; ++c) {
                    if (!row_bits[p][c]) {
                        newdig[c] = 0;
                        continue;
                    }
                    int left  = (c > 0)    && row_bits[p][c - 1];
                    int right = (c < WIDTH - 1) && row_bits[p][c + 1];
                    int up    = state_digits[s][c] != 0;
                    int count = up + left + right;
                    if (count > 3) {     /* would exceed the maximum */
                        ok = false;
                        break;
                    }
                    newdig[c] = (uint8_t)(count + 1); /* digits 1…4 → count 0…3 */
                }
                if (ok) {
                    for (int c = 0; c < WIDTH; ++c)
                        new_state += newdig[c] * pow5[c];
                }
            }

            trans_state[s][p] = ok ? (int16_t)new_state : -1;
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();

    /* static state for memoisation -------------------------------- */
    static seq_t *dp    = NULL;          /* counts for current
                                            processed length   */
    static seq_t *newdp = NULL;
    static idx_t cur_len = 0;            /* number of rows processed
                                            so far             */
    static seq_t *values = NULL;         /* a(1)…a(cur_len) stored   */
    static idx_t vals_sz = 0;            /* allocated size of values  */

    /* Allocate DP arrays on first use */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * STATE_CNT);
        newdp = (seq_t *)malloc(sizeof(seq_t) * STATE_CNT);
        for (int i = 0; i < STATE_CNT; ++i)
            dp[i] = 0;
        dp[0] = 1;                     /* empty board */
        cur_len = 0;

        /* values[0] unused; allocate space for term 1 */
        values = (seq_t *)malloc(sizeof(seq_t) * 1);
        values[0] = 0;
        vals_sz = 1;
    }

    /* Grow the values array if necessary */
    if (n + 1 > vals_sz) {
        vals_sz = n + 1;
        values = (seq_t *)realloc(values, sizeof(seq_t) * vals_sz);
    }

    /* Compute terms up to n if needed */
    while (cur_len < n) {
        /* Clear newdp */
        for (int i = 0; i < STATE_CNT; ++i)
            newdp[i] = 0;

        /* Transition from each existing state to all row patterns */
        for (int s = 0; s < STATE_CNT; ++s) {
            if (dp[s] == 0)
                continue;
            for (int p = 0; p < TRANS_CNT; ++p) {
                int16_t ns = trans_state[s][p];
                if (ns >= 0)
                    newdp[ns] = newdp[ns] + dp[s];
            }
        }

        /* Swap dp and newdp for the next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;

        /* Sum the counts of states that are valid final rows */
        seq_t sum = 0;
        for (int s = 0; s < STATE_CNT; ++s) {
            if (dp[s] != 0 && last_valid[s])
                sum = sum + dp[s];
        }

        values[cur_len + 1] = sum;
        ++cur_len;
    }

    return values[n];
}
