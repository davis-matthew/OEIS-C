/*
 * A251197  Number of (n+1) X (4+1) 0..1 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements less than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of (n+1)×5 binary arrays with no 2×2 block
   for which the minimum of its diagonal elements is less than the
   absolute difference of its antidiagonal elements.
   The rule for an adjacent pair of rows (s top, t bottom) is
   (b==c) || (a==1 && d==1) on each pair of adjacent columns.
   The sequence is computed by dynamic programming over rows. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    enum { STATES = 32 };                 /* 5-bit rows */
    static bool adj[STATES][STATES];
    static bool adj_init = false;

    if (!adj_init) {
        /* build adjacency matrix */
        for (int s = 0; s < STATES; ++s) {
            for (int t = 0; t < STATES; ++t) {
                bool ok = true;
                for (int j = 0; j < 4; ++j) {
                    int a_bit = (s >> j)       & 1; /* top-left   */
                    int b_bit = (s >> (j + 1)) & 1; /* top-right  */
                    int c_bit = (t >> j)       & 1; /* bottom-left*/
                    int d_bit = (t >> (j + 1)) & 1; /* bottom-right*/
                    if (!( (b_bit == c_bit) || (a_bit == 1 && d_bit == 1) )) {
                        ok = false;
                        break;
                    }
                }
                adj[s][t] = ok;
            }
        }
        adj_init = true;
    }

    seq_t dp_prev[STATES];
    seq_t dp_cur [STATES];

    /* initial state: any single row allowed */
    for (int i = 0; i < STATES; ++i)
        dp_prev[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {   /* add rows */
        for (int t = 0; t < STATES; ++t)
            dp_cur[t] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_prev[s];
            if (!val) continue;
            for (int t = 0; t < STATES; ++t) {
                if (adj[s][t])
                    dp_cur[t] += val;
            }
        }

        /* copy dp_cur to dp_prev for next iteration */
        for (int i = 0; i < STATES; ++i)
            dp_prev[i] = dp_cur[i];
    }

    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += dp_prev[i];

    return total;
}
