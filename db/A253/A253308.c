/*
 * A253308  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock diagonal
 *          maximum minus antidiagonal maximum unequal to its neighbors
 *          horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..2 such that for
 *            every 2×2 subblock the value (diagonal‑max – anti‑diagonal‑max)
 *            differs from that of all four neighboring subblocks
 *            (horizontally, vertically, diagonally and anti‑diagonally).
 *            Computed by dynamic programming over all possible pairs o...
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   pair_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pair_diff.h"

/* ------------------------------------------------------------------ */
/*  Precomputation (run once)                                         */
/* ------------------------------------------------------------------ */

#define ROWS   27              /* 3³ possible 3‑digit rows (0..2) */
#define STATES (ROWS * ROWS)   /* pair of rows → state index */

static bool         state_valid[STATES]; /* diff_left != diff_right for pair */
static int8_t       diff_val[ROWS][ROWS][2]; /* diff for each pair, left/right */
static uint8_t      next_cnt[STATES];      /* number of outgoing transitions */
static int          next_state[STATES][ROWS]; /* adjacency list */

/* Convert an integer index to its 3 digit representation (0‑2). */
static void row_digits(int idx, int digit[3]) {
    int t = idx;
    digit[0] = t % 3; t /= 3;
    digit[1] = t % 3; t /= 3;
    digit[2] = t;                    /* last digit is already 0..2 */
}

/* Initialise all static tables (thread‑safe for single‑thread environment). */
static void init(void) {
    static bool inited = false;
    if (inited) return;
    inited = true;

    /* Expand all row indices into digit arrays. */
    int row_digit[ROWS][3];
    for (int r = 0; r < ROWS; ++r) row_digits(r, row_digit[r]);

    /* Pre‑compute differences for each ordered pair of rows. */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            diff_val[a][b][0] = pair_diff(row_digit[a], row_digit[b], 0);
            diff_val[a][b][1] = pair_diff(row_digit[a], row_digit[b], 1);
            state_valid[a * ROWS + b] =
                (diff_val[a][b][0] != diff_val[a][b][1]);   /* left ≠ right */
        }
    }

    /* Build adjacency lists subject to the four neighbour constraints. */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            int s_from = a * ROWS + b;
            if (!state_valid[s_from]) { next_cnt[s_from] = 0; continue; }

            int8_t old0 = diff_val[a][b][0];
            int8_t old1 = diff_val[a][b][1];
            unsigned cnt = 0;

            for (int c = 0; c < ROWS; ++c) {
                int8_t new0 = diff_val[b][c][0];
                int8_t new1 = diff_val[b][c][1];

                if (new0 == new1)                 continue; /* horizontal */
                if (new0 == old0 || new0 == old1) continue; /* vertical / diagonal */
                if (new1 == old1 || new1 == old0) continue; /* vertical / anti‑diagonal */

                next_state[s_from][cnt++] = b * ROWS + c;
            }
            next_cnt[s_from] = cnt;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence generator                                                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init();                               /* ensure tables are ready */

    /* Number of rows in the array is n+1.  For n=1 we have 2 rows. */
    idx_t rows = n + 1;
    if (rows < 2) return 0;               /* invalid but not used */

    /* Allocate DP tables on the stack; they are default‑constructed. */
    seq_t dp[STATES]   = {0};
    seq_t nxt[STATES];

    /* ---- initial states: any pair of first two rows satisfying the
       horizontal constraint. All such states start with count 1. ---- */
    for (int s = 0; s < STATES; ++s) {
        dp[s] = state_valid[s] ? (seq_t)1 : (seq_t)0;
    }

    /* pointers to current and next dp arrays (to avoid memcpy of objects) */
    seq_t *cur = dp;
    seq_t *nextptr = nxt;

    /* ---- propagate through the remaining rows ---- */
    for (idx_t step = 0; step < rows - 2; ++step) {
        /* clear next array */
        for (size_t i = 0; i < STATES; ++i) nextptr[i] = (seq_t)0;

        /* transition */
        for (int s = 0; s < STATES; ++s) {
            if (cur[s] == 0) continue;
            seq_t val = cur[s];
            uint8_t cnt = next_cnt[s];
            for (uint8_t k = 0; k < cnt; ++k) {
                int ns = next_state[s][k];
                nextptr[ns] += val;
            }
        }

        /* swap current and next */
        seq_t *tmp = cur; cur = nextptr; nextptr = tmp;
    }

    /* ---- sum all final states that are valid. ---- */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s) {
        if (state_valid[s]) result += cur[s];
    }
    return result;
}
