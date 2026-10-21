/*
 * A252442  Number of (n+2)X(2+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum less than 3.
 * https://oeis.org/A252442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A252442
   Number of (n+2)×4 0..2 arrays with every 3×3 subblock:
   each row sum, each column sum, main diagonal sum and antidiagonal sum
   are less than 3.
   Computation is via dynamic programming over the last two rows. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define ROWS   81          /* 3^4 possible rows of length 4 */
#define STATES (ROWS * ROWS)

static uint8_t row_vals[ROWS][4];          /* decoded row values */
static int    adj_len[STATES];             /* number of admissible next rows per state */
static uint8_t *adj[STATES];              /* indices of admissible next rows */

static bool init_done = false;

/* Return true if the 3×3 window defined by rows r0,r1,r2 starting at column j
   satisfies the row, column, diagonal and antidiagonal sum constraints. */
static bool window_ok(const uint8_t *r0, const uint8_t *r1, const uint8_t *r2, int j)
{
    /* Row sums */
    int s0 = r0[j] + r0[j+1] + r0[j+2];
    if (s0 >= 3) return false;
    int s1 = r1[j] + r1[j+1] + r1[j+2];
    if (s1 >= 3) return false;
    int s2 = r2[j] + r2[j+1] + r2[j+2];
    if (s2 >= 3) return false;

    /* Column sums */
    int c0 = r0[j] + r1[j] + r2[j];
    if (c0 >= 3) return false;
    int c1 = r0[j+1] + r1[j+1] + r2[j+1];
    if (c1 >= 3) return false;
    int c2 = r0[j+2] + r1[j+2] + r2[j+2];
    if (c2 >= 3) return false;

    /* Diagonals */
    int d  = r0[j]   + r1[j+1] + r2[j+2];
    if (d >= 3) return false;
    int ad = r0[j+2] + r1[j+1] + r2[j];
    if (ad >= 3) return false;

    return true;
}

/* Build lookup tables and transition lists once. */
static void init(void)
{
    if (init_done) return;

    /* Encode all 81 rows. */
    for (int idx = 0; idx < ROWS; ++idx) {
        int v = idx;
        for (int col = 0; col < 4; ++col) {
            row_vals[idx][col] = v % 3;
            v /= 3;
        }
    }

    /* Build adjacency lists for each state (prev2, prev1). */
    for (int s = 0; s < STATES; ++s) {
        int prev2 = s / ROWS;
        int prev1 = s % ROWS;
        int cnt = 0;
        for (int cur = 0; cur < ROWS; ++cur) {
            if (window_ok(row_vals[prev2], row_vals[prev1], row_vals[cur], 0) &&
                window_ok(row_vals[prev2], row_vals[prev1], row_vals[cur], 1))
                ++cnt;
        }
        adj_len[s] = cnt;
        if (cnt > 0) {
            adj[s] = (uint8_t *)malloc(cnt * sizeof(uint8_t));
            int pos = 0;
            for (int cur = 0; cur < ROWS; ++cur) {
                if (window_ok(row_vals[prev2], row_vals[prev1], row_vals[cur], 0) &&
                    window_ok(row_vals[prev2], row_vals[prev1], row_vals[cur], 1))
                    adj[s][pos++] = (uint8_t)cur;
            }
        } else {
            adj[s] = NULL;
        }
    }

    init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init();                              /* build tables once */

    /* DP arrays for current and next state counts */
    static seq_t dp0[STATES];
    static seq_t dp1[STATES];
    seq_t *cur = dp0;
    seq_t *nxt = dp1;

    /* Zero initialize */
    for (size_t i = 0; i < STATES; ++i) cur[i] = 0;

    /* All pairs of first two rows are admissible. */
    for (size_t a = 0; a < ROWS; ++a) {
        for (size_t b = 0; b < ROWS; ++b) {
            cur[a * ROWS + b] = 1;
        }
    }

    /* The array has height h = n + 2 rows; we have already placed 2 rows. */
    size_t steps = (size_t)n;          /* number of additional rows to add */

    for (size_t step = 0; step < steps; ++step) {
        /* reset nxt to zero */
        for (size_t i = 0; i < STATES; ++i) nxt[i] = 0;

        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            size_t prev1 = s % ROWS;
            for (int k = 0; k < adj_len[s]; ++k) {
                size_t currow = adj[s][k];
                size_t nextstate = prev1 * ROWS + currow;
                nxt[nextstate] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all state counts to get the answer. */
    seq_t ans = 0;
    for (size_t i = 0; i < STATES; ++i) ans += cur[i];
    return ans;
}
