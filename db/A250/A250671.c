/*
 * A250671  Number of (n+1)X(3+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+1)×(4) arrays with entries 0..3 such that (1) for
 *            each row i: x(i,j)+x(i,j‑1) is non‑decreasing in j for j =
 *            1,2,3; (2) for each column j: |x(i,j) – x(i‑1,j)| is
 *            non‑decreasing in i for i = 1,2,…; These arrays are counted by
 *            dynamic programming on the 4‑digit rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 22, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Pre‑computed tables – built once, reused by all calls. */
static unsigned char row_vals[256][4];      /* digits for each state (0..3) */
static unsigned char adj_count[256];       /* number of predecessors per state */
static unsigned short adj[256][256];       /* list of predecessors per state */

/* Build tables on first call. */
static void init_tables(void)
{
    static bool initialized = false;
    if (initialized) return;

    /* Decode each state into its four digits. */
    for (unsigned int s = 0; s < 256; ++s) {
        unsigned int t = s;
        for (int j = 3; j >= 0; --j) {
            row_vals[s][j] = t & 3;
            t >>= 2;
        }
    }

    /* Build adjacency lists: for each cur state store all prev states
       that satisfy both constraints. */
    for (unsigned int prev = 0; prev < 256; ++prev) {
        for (unsigned int cur = 0; cur < 256; ++cur) {
            bool ok = true;

            /* Condition 1: rowwise sums are nondecreasing. */
            for (int j = 1; j <= 3 && ok; ++j) {
                unsigned int sum_cur  = row_vals[cur][j]   + row_vals[cur][j-1];
                unsigned int sum_prev = row_vals[prev][j]  + row_vals[prev][j-1];
                if (sum_cur < sum_prev) ok = false;
            }
            if (!ok) continue;

            /* Condition 2: absolute differences are nondecreasing in j. */
            unsigned int d0 = row_vals[cur][0] > row_vals[prev][0]
                                ? row_vals[cur][0] - row_vals[prev][0]
                                : row_vals[prev][0] - row_vals[cur][0];
            unsigned int d1 = row_vals[cur][1] > row_vals[prev][1]
                                ? row_vals[cur][1] - row_vals[prev][1]
                                : row_vals[prev][1] - row_vals[cur][1];
            unsigned int d2 = row_vals[cur][2] > row_vals[prev][2]
                                ? row_vals[cur][2] - row_vals[prev][2]
                                : row_vals[prev][2] - row_vals[cur][2];
            unsigned int d3 = row_vals[cur][3] > row_vals[prev][3]
                                ? row_vals[cur][3] - row_vals[prev][3]
                                : row_vals[prev][3] - row_vals[cur][3];
            if (d0 > d1 || d1 > d2 || d2 > d3) continue;

            /* Store the transition. */
            unsigned char idx = adj_count[cur];
            adj[cur][idx] = (unsigned short)prev;
            adj_count[cur] = idx + 1;
        }
    }

    initialized = true;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    init_tables();

    seq_t dp_prev[256];
    seq_t dp_curr[256];

    /* Any first row is allowed. */
    for (int i = 0; i < 256; ++i) dp_prev[i] = (seq_t)1;

    /* Perform n transitions (rows 2 … n+1). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int cur = 0; cur < 256; ++cur) {
            seq_t sum = (seq_t)0;
            unsigned char cnt = adj_count[cur];
            for (unsigned char k = 0; k < cnt; ++k) {
                sum += dp_prev[adj[cur][k]];
            }
            dp_curr[cur] = sum;
        }
        for (int i = 0; i < 256; ++i) dp_prev[i] = dp_curr[i];
    }

    /* Sum over all possible final rows. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i) result += dp_prev[i];
    return result;
}
