/*
 * A253656  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock diagonal
 *          maximum minus antidiagonal maximum unequal to its neighbors
 *          horizontally, vertically and nw-se diagonally.
 * https://oeis.org/A253656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253656 – number of (n+1)×3 arrays of 0..2 such that
   for every 2×2 subblock the value
        D = max(diagonal) – max(anti‑diagonal)
   is unequal to its neighbours horizontally, vertically and NW–SE diagonally.
   The algorithm counts arrays by dynamic programming over the row sequence.
   State = (last row, D of previous pair's first block, D of previous pair's second block).
   Transition checks the four inequalities involving the previous D values.
   Complexity O(n · 675 · 27), well below the limit for n≈10^4. */
    
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define ROWS 27          /* 3^3 possible rows */
#define D_RANGE 5        /* D ∈ {‑2,‑1,0,1,2} */
#define DOFF 2           /* offset for negative indices */
#define STATE_COUNT (ROWS * D_RANGE * D_RANGE)

/* Maps a state to an integer 0..STATE_COUNT‑1 */
static inline int state_idx(int row, int d0, int d1)
{
    return row * (D_RANGE * D_RANGE) + (d0 + DOFF) * D_RANGE + (d1 + DOFF);
}

/* Pre‑computed data, initialised only once */
static bool init_done = false;
static int8_t row_digit[ROWS][3];
static int8_t pair_d0[ROWS][ROWS], pair_d1[ROWS][ROWS];
static bool  state_valid[STATE_COUNT];
static int16_t trans[STATE_COUNT][ROWS];

/* DP arrays */
static seq_t dpA[STATE_COUNT];
static seq_t dpB[STATE_COUNT];

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at n=1 */

    /* ---- initialise static tables on first call -------------------------------- */
    if (!init_done) {
        /* convert each 0..26 index to its ternary digits (least significant first) */
        for (int r = 0; r < ROWS; ++r) {
            int t = r;
            row_digit[r][0] = t % 3; t /= 3;
            row_digit[r][1] = t % 3; t /= 3;
            row_digit[r][2] = t % 3;
        }

        /* pre‑compute D for every ordered pair of rows, marking invalid pairs
           (where the two D values are equal) by setting -5 */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                /* block c = 0 : columns 0&1 */
                int d0 = (row_digit[a][0] > row_digit[b][1] ? row_digit[a][0] : row_digit[b][1]) -
                         (row_digit[a][1] > row_digit[b][0] ? row_digit[a][1] : row_digit[b][0]);

                /* block c = 1 : columns 1&2 */
                int d1 = (row_digit[a][1] > row_digit[b][2] ? row_digit[a][1] : row_digit[b][2]) -
                         (row_digit[a][2] > row_digit[b][1] ? row_digit[a][2] : row_digit[b][1]);

                if (d0 == d1) {
                    pair_d0[a][b] = -5;
                    pair_d1[a][b] = -5;
                } else {
                    pair_d0[a][b] = (int8_t)d0;
                    pair_d1[a][b] = (int8_t)d1;
                }
            }
        }

        /* mark valid states and pre‑compute transitions */
        for (int row = 0; row < ROWS; ++row) {
            for (int d0 = -2; d0 <= 2; ++d0) {
                for (int d1 = -2; d1 <= 2; ++d1) {
                    int s = state_idx(row, d0, d1);
                    state_valid[s] = false;                 /* default */

                    /* A state is *complete* only if it was reached by a valid pair.
                       We will fill state_valid later via transitions.               */
                }
            }
        }

        /* Build transitions: from a state (row_prev,d_prev0,d_prev1)
           to a state (row_next,d_next0,d_next1).  */
        for (int s = 0; s < STATE_COUNT; ++s) {
            for (int next = 0; next < ROWS; ++next)
                trans[s][next] = -1;   /* initialise with no transition */

            int prev_row = s / (D_RANGE * D_RANGE);
            int rem     = s % (D_RANGE * D_RANGE);
            int d_prev0 = rem / D_RANGE - DOFF;
            int d_prev1 = rem % D_RANGE - DOFF;

            for (int next_row = 0; next_row < ROWS; ++next_row) {
                int8_t d0_new = pair_d0[prev_row][next_row];
                int8_t d1_new = pair_d1[prev_row][next_row];
                if (d0_new == -5) continue;           /* invalid pair, skip */

                /* inequalities that must hold between consecutive pairs */
                if (d_prev0 != d0_new && d_prev1 != d1_new && d_prev1 != d0_new) {
                    int ns = state_idx(next_row, d0_new, d1_new);
                    trans[s][next_row] = (int16_t)ns;
                }
            }
        }

        /* Mark valid states: those that can be an ending state of a pair
           (i.e., d0 != d1 for the corresponding pair). */
        for (int s = 0; s < STATE_COUNT; ++s) {
            int row = s / (D_RANGE * D_RANGE);
            int rem = s % (D_RANGE * D_RANGE);
            int d0  = rem / D_RANGE - DOFF;
            int d1  = rem % D_RANGE - DOFF;
            state_valid[s] = (d0 != d1);
        }

        init_done = true;
    }

    /* ---- initialisation: first pair ------------------------------------------- */
    memset(dpA, 0, sizeof(dpA));
    if (n >= 1) {
        for (int r0 = 0; r0 < ROWS; ++r0) {
            for (int r1 = 0; r1 < ROWS; ++r1) {
                if (pair_d0[r0][r1] == -5) continue;   /* d0 == d1, invalid */
                int s = state_idx(r1, pair_d0[r0][r1], pair_d1[r0][r1]);
                dpA[s] += 1;
            }
        }
    }
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s) total += dpA[s];
        return total;
    }

    /* ---- dynamic programming over remaining (n‑1) transitions ----------------- */
    seq_t *curr = dpA;
    seq_t *next = dpB;
    for (idx_t step = 2; step <= n; ++step) {
        memset(next, 0, sizeof(dpB));
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = curr[s];
            if (val == 0) continue;
            for (int r = 0; r < ROWS; ++r) {
                int ns = trans[s][r];
                if (ns < 0) continue;
                next[ns] += val;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = curr; curr = next; next = tmp;
    }

    /* ---- sum all ending states --------------------------------------------- */
    seq_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s) total += curr[s];
    return total;
}
