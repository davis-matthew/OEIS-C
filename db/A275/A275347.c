/*
 * A275347  Number of nX3 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,-2) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×3 arrays over {0,1,2} that are 1)
 *            restricted‑growth (new values appear in order 0,1,2), and 2) no
 *            cell equals any of its offset neighbours (−2,−1), (−2,+1),
 *            (−1,−2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS   27                /* 0..26 are real rows, 27 = “empty” sentinel   */
#define SENTINEL   27
#define MAX_NU     4                 /* new value that is not yet used (0..3)        */
#define PP_DIM     28                /* prevprev: 0..27 (with 27=sentinel)           */
#define P_DIM      28                /* prev:    0..27                                */
#define NU_DIM     4                 /* nu:      0..3                                 */
#define STATE_COUNT (PP_DIM * P_DIM * NU_DIM)

/* Helper to encode a state into an array index */
static inline idx_t STATE_IDX(int pp, int p, int nu) {
    return ((idx_t)pp * P_DIM + (idx_t)p) * NU_DIM + (idx_t)nu;
}

/* Table of 3 digits for each real row (0‑based) */
static unsigned char row_vals[MAX_ROWS][3];

/* Pre‑compute row values (base‑3 digits) */
static void init_row_vals(void) {
    for (int r = 0; r < MAX_ROWS; ++r) {
        int tmp = r;
        row_vals[r][0] = tmp % 3;          // column 0
        tmp /= 3;
        row_vals[r][1] = tmp % 3;          // column 1
        tmp /= 3;
        row_vals[r][2] = tmp % 3;          // column 2
    }
}

/* Check whether adding a new row is valid and compute the new 'nu' */
static bool row_is_valid(int new_row,
                         int pp, int p,
                         int nu,      /* current next unused value */
                         int *new_nu) /* computed new next unused value */
{
    int cur_nu = nu;
    for (int col = 0; col < 3; ++col) {
        int v = row_vals[new_row][col];

        /* Restricted‑growth rule */
        if (v > cur_nu) return false;
        if (v == cur_nu) cur_nu++;

        /* Adjacency constraints */

        /* (−1,−2) : previous row, column−2 */
        if (p != SENTINEL && col >= 2) {
            int av = row_vals[p][col - 2];
            if (v == av) return false;
        }

        /* (−2,−1) : prevprev row, column−1 */
        if (pp != SENTINEL && col >= 1) {
            int av = row_vals[pp][col - 1];
            if (v == av) return false;
        }

        /* (−2,+1) : prevprev row, column+1 */
        if (pp != SENTINEL && col <= 1) {
            int av = row_vals[pp][col + 1];
            if (v == av) return false;
        }
    }
    *new_nu = cur_nu;
    return true;
}

/* The function required by the exercise */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *curr = NULL;        /* distribution after the last computed number of rows */
    static seq_t *next = NULL;        /* temporary buffer for the next step */
    static idx_t computed_rows = 0;   /* number of rows already processed (>=0) */
    static seq_t *cumulative = NULL;  /* cumulative totals for n=1..max_computed */
    static size_t cum_cap = 0;

    if (!initialized) {
        init_row_vals();
        curr = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
        next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
        if (!curr || !next) return (seq_t)0; /* allocation failure */
        /* initial state: no rows processed */
        curr[STATE_IDX(SENTINEL, SENTINEL, 0)] = (seq_t)1;
        initialized = true;
    }

    if (n == 0) return (seq_t)0;            /* offset is 1, but guard needed */

    /* If request is already satisfied */
    if (n <= computed_rows) {
        return cumulative[n - 1];
    }

    /* Extend computations up to n */
    for (idx_t rows = computed_rows; rows < n; ++rows) {

        /* zero the next buffer */
        memset(next, 0, STATE_COUNT * sizeof(seq_t));

        /* Loop over all states of the current distribution */
        for (int pp = 0; pp < PP_DIM; ++pp) {
            for (int p = 0; p < P_DIM; ++p) {
                for (int nu = 0; nu < NU_DIM; ++nu) {
                    seq_t cnt = curr[STATE_IDX(pp, p, nu)];
                    if (cnt == (seq_t)0) continue;

                    /* Try all possible new rows */
                    for (int new_row = 0; new_row < MAX_ROWS; ++new_row) {
                        int new_nu;
                        if (!row_is_valid(new_row, pp, p, nu, &new_nu))
                            continue;
                        int ns = STATE_IDX(p, new_row, new_nu);
                        next[ns] = next[ns] + cnt;
                    }
                }
            }
        }

        /* Swap buffers: next becomes current for the next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;

        /* Increment row counter */
        computed_rows = rows + 1;

        /* Compute total for this many rows */
        seq_t total = (seq_t)0;
        for (idx_t i = 0; i < STATE_COUNT; ++i)
            total += curr[i];

        /* Store cumulative total */
        if (computed_rows > cum_cap) {
            size_t new_cap = cum_cap ? cum_cap * 2 : 16;
            while (new_cap < computed_rows) new_cap *= 2;
            seq_t *new_arr = (seq_t *)realloc(cumulative, new_cap * sizeof(seq_t));
            if (!new_arr) return (seq_t)0; /* allocation failure */
            cumulative = new_arr;
            cum_cap = new_cap;
        }
        cumulative[computed_rows - 1] = total;
    }

    return cumulative[n - 1];
}
