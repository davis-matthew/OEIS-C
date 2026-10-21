/*
 * A252497  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum less than 5.
 * https://oeis.org/A252497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+2)×3 arrays with entries 0..3 such that
 *            every 3×3 subblock has all row sums, column sums, diagonal sum
 *            and anti‑diagonal sum < 5. The implementation builds a directed
 *            graph whose vertices are ordered pairs of rows (r₁,r₂) [0≤r≤63],
 *            and an edge (r₁,r₂)→(r₂,r₃) exists iff t...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* state constants */
#define NUM_ROWS 64                      /* 4³ possible rows */
#define NUM_STATES (NUM_ROWS * NUM_ROWS) /* ordered pairs of rows */

/* global tables (static storage) */
static uint8_t  row_dig[NUM_ROWS][3];
static uint8_t  row_sum[NUM_ROWS];
static uint8_t  outdeg[NUM_STATES];
static uint8_t  edge_r3[NUM_STATES][64];   /* child row                                 */
static uint16_t edge_next[NUM_STATES][64]; /* target state (r₂,r₃) as index             */
static bool     graph_built = false;

/* ---------------------------------------------------------------------- */
/* Build row data and graph adjacency only once                                   */
static void build_graph(void)
{
    /* pre‑compute row digit strings and sums */
    for (int r = 0; r < NUM_ROWS; ++r) {
        int tmp = r;
        row_dig[r][0] = tmp & 3;           /* least significant digit   */
        row_dig[r][1] = (tmp >> 2) & 3;
        row_dig[r][2] = (tmp >> 4) & 3;    /* most significant digit    */
        row_sum[r] = row_dig[r][0] + row_dig[r][1] + row_dig[r][2];
    }

    /* build adjacency list */
    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            int state = (r1 << 6) | r2;    /* encode pair */
            int deg = 0;
            for (int r3 = 0; r3 < NUM_ROWS; ++r3) {

                /* validity checks for the triple (r1,r2,r3) */
                if (row_sum[r1] > 4 || row_sum[r2] > 4 || row_sum[r3] > 4) continue;

                int c0 = row_dig[r1][0] + row_dig[r2][0] + row_dig[r3][0];
                if (c0 > 4) continue;
                int c1 = row_dig[r1][1] + row_dig[r2][1] + row_dig[r3][1];
                if (c1 > 4) continue;
                int c2 = row_dig[r1][2] + row_dig[r2][2] + row_dig[r3][2];
                if (c2 > 4) continue;

                int diag   = row_dig[r1][0] + row_dig[r2][1] + row_dig[r3][2];
                if (diag > 4) continue;
                int antiDi = row_dig[r1][2] + row_dig[r2][1] + row_dig[r3][0];
                if (antiDi > 4) continue;

                /* store edge */
                edge_r3[state][deg]   = (uint8_t)r3;
                edge_next[state][deg] = (uint16_t)((r2 << 6) | r3);
                ++deg;
            }
            outdeg[state] = (uint8_t)deg;
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Main sequence function                                                     */
seq_t a(idx_t n)
{
    /* build graph once */
    if (!graph_built) {
        build_graph();
        graph_built = true;
    }

    /* DP arrays: local to the function to avoid interference between calls */
    static seq_t cur[NUM_STATES];
    static seq_t nxt[NUM_STATES];
    seq_t *c = cur;
    seq_t *d = nxt;

    /* initialise: for any starting pair (r0,r1) there is one way before
       the first 3×3 constraint is applied */
    for (int i = 0; i < NUM_STATES; ++i) {
        c[i] = (seq_t)1;
        d[i] = (seq_t)0;
    }

    /* number of transitions = n (since we add n rows after the first two) */
    for (idx_t step = 0; step < n; ++step) {
        /* clear destination array */
        memset(d, 0, NUM_STATES * sizeof(seq_t));

        for (int s = 0; s < NUM_STATES; ++s) {
            seq_t val = c[s];
            if (val == (seq_t)0) continue;
            uint8_t deg = outdeg[s];
            for (uint8_t k = 0; k < deg; ++k) {
                uint16_t t = edge_next[s][k];
                d[t] += val;
            }
        }
        /* swap current and destination */
        seq_t *tmp = c;
        c = d;
        d = tmp;
    }

    /* sum over all final states to get total number of arrays */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < NUM_STATES; ++i) {
        ans += c[i];
    }
    return ans;
}
