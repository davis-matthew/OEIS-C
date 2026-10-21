/*
 * A251830  Number of (n+2) X (1+2) 0..3 arrays with no row, column, diagonal
 *          or antidiagonal in any 3 X 3 subblock summing to 3 4 5 or 6.
 * https://oeis.org/A251830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A251830
 *
 * The sequence counts the number of (n+2)×3 arrays with entries in {0,1,2,3}
 * such that in every 3×3 subblock
 *   • no row sum,
 *   • no column sum,
 *   • no main diagonal sum,
 *   • no anti‑diagonal sum
 * is equal to 3, 4, 5 or 6.
 *
 * The algorithm builds all “valid” row patterns (their sum is outside 3…6),
 * and then uses dynamic programming on pairs of consecutive rows.
 * For each state (previous two rows) we pre‑compute the set of rows that can
 * follow it while keeping the new 3×3 block valid.  The number of arrays
 * of height n+2 is obtained by iterating this transition n times.
 *
 * The function is memoised only by a one‑time global initialisation; it works
 * correctly for any call order and any n ≥ 1.  It runs in O(n) time and
 * uses O(ROWS^2) memory (here ROWS = 20, so at most 400 states). */

#include <assert.h>

typedef idx_t idx_t;   /* integer index type provided by the build */
typedef seq_t seq_t;   /* integer value type provided by the build */

#define ROWS     20          /* number of row patterns whose sum is ≤2 or ≥7 */
#define STATES  (ROWS * ROWS)/* number of states = (previous two rows) */

static int        valid_rows[ROWS];          /* masks of valid rows (3 cells, 2 bits each) */
static int        row_vals[ROWS][3];         /* decoded cell values per row */
static int        trans[STATES][ROWS];       /* next‑row indices for each state   */
static unsigned char deg[STATES];            /* number of transitions per state  */
static int        init_done = 0;

/* Initialise the row patterns and transition tables once */
static void init(void)
{
    if (init_done) return;
    int rcnt = 0;
    for (int mask = 0; mask < 64; ++mask) {
        int v0 =  mask       & 3;
        int v1 = (mask >> 2) & 3;
        int v2 = (mask >> 4) & 3;
        int sum = v0 + v1 + v2;
        if (sum <= 2 || sum >= 7) {
            valid_rows[rcnt] = mask;
            row_vals[rcnt][0] = v0;
            row_vals[rcnt][1] = v1;
            row_vals[rcnt][2] = v2;
            ++rcnt;
        }
    }
    assert(rcnt == ROWS);   /* sanity check: should be 20 */

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int state = i * ROWS + j;
            unsigned char d = 0;
            for (int k = 0; k < ROWS; ++k) {
                int c0 = row_vals[i][0] + row_vals[j][0] + row_vals[k][0];
                int c1 = row_vals[i][1] + row_vals[j][1] + row_vals[k][1];
                int c2 = row_vals[i][2] + row_vals[j][2] + row_vals[k][2];
                int d1 = row_vals[i][0] + row_vals[j][1] + row_vals[k][2];
                int d2 = row_vals[i][2] + row_vals[j][1] + row_vals[k][0];
                if ((c0 <= 2 || c0 >= 7) &&
                    (c1 <= 2 || c1 >= 7) &&
                    (c2 <= 2 || c2 >= 7) &&
                    (d1 <= 2 || d1 >= 7) &&
                    (d2 <= 2 || d2 >= 7)) {
                    trans[state][d++] = k;
                }
            }
            deg[state] = d;
        }
    }
    init_done = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1; safe to return 0 */

    init();   /* ensure tables are built once */

    const idx_t rows = n + 2;   /* total number of rows in the array */
    const idx_t steps = rows - 2; /* number of rows to add after the first two */

    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* All pairs of initial rows are allowed */
    for (int s = 0; s < STATES; ++s) dp_cur[s] = 1;

    for (idx_t step = 0; step < steps; ++step) {
        /* clear dp_next */
        for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cur = dp_cur[s];
            if (cur == 0) continue;
            unsigned char d = deg[s];
            int j = s % ROWS;       /* second row index of the current pair */
            for (int idx = 0; idx < d; ++idx) {
                int k = trans[s][idx];
                int ns = j * ROWS + k;
                dp_next[ns] += cur;
            }
        }
        /* advance to next step */
        for (int s = 0; s < STATES; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum all valid sequences of full length */
    seq_t total = 0;
    for (int s = 0; s < STATES; ++s) total += dp_cur[s];
    return total;
}
