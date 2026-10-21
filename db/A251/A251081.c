/*
 * A251081  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A251081: Number of (n+1) X 2 0..2 arrays with no 2×2 subblock
   having sum of its diagonal elements greater than the absolute
   difference of its antidiagonal elements.
   Implementation: dynamic programming on a 9×9 compatibility graph.
   State: a pair (a,b) with a,b∈{0,1,2}, encoded as
            state = a*3 + b   (0 ≤ state < 9)
   Transition: state s = (a,b) can be followed by state t = (c,d) if
               a + d ≤ |b – c|.
   The sequence is a(n) = sum_{t} (A^n)[t][*] where A is the
   compatibility matrix.  We compute it iteratively for each
   requested n, which is O(n) per call and easily fast for n≤10000.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compatibility matrix for 9 possible row states */
    static const unsigned char compat[9][9] = {
        /*   t:  0  1  2  3  4  5  6  7  8  (c,d) */
        /* s=0 (0,0) */
        {1,1,1,1,1,1,1,1,1},
        /* s=1 (0,1) */
        {1,1,1,1,1,0,1,1,0},
        /* s=2 (0,2) */
        {1,1,1,1,1,1,1,1,1},
        /* s=3 (1,0) */
        {1,1,1,1,1,1,1,1,1},
        /* s=4 (1,1) */
        {1,0,1,1,1,1,1,1,0},
        /* s=5 (1,2) */
        {1,1,1,1,1,1,1,1,1},
        /* s=6 (2,0) */
        {1,1,1,1,1,1,1,1,1},
        /* s=7 (2,1) */
        {1,0,1,1,1,1,1,1,0},
        /* s=8 (2,2) */
        {1,1,1,1,1,1,1,1,1}
    };

    /* Pre‑compute the actual compatibility matrix */
    static unsigned char table[9][9];
    static int initialized = 0;
    if (!initialized) {
        for (int s = 0; s < 9; ++s) {
            int a = s / 3;
            int b = s % 3;
            for (int t = 0; t < 9; ++t) {
                int c = t / 3;
                int d = t % 3;
                int diagSum = a + d;
                int antiDiff = b >= c ? b - c : c - b;
                table[s][t] = (diagSum <= antiDiff) ? 1 : 0;
            }
        }
        initialized = 1;
    }

    seq_t curr[9];
    seq_t next[9];
    for (int i = 0; i < 9; ++i) curr[i] = 1;          /* length 1 row */

    for (idx_t step = 1; step <= n; ++step) {
        for (int t = 0; t < 9; ++t) {
            seq_t sum = 0;
            for (int s = 0; s < 9; ++s)
                if (table[s][t]) sum += curr[s];
            next[t] = sum;
        }
        if (step == n) {
            /* Sum over all ending states */
            seq_t total = 0;
            for (int t = 0; t < 9; ++t) total += next[t];
            return total;
        }
        for (int t = 0; t < 9; ++t) curr[t] = next[t];
    }
    return 0;   /* Should never reach here for n ≥ 1 */
}
