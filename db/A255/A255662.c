/*
 * A255662  Number of length n+2 0..3 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length n+2 arrays over {0,1,2,3} with at most two downsteps
   in the two windows of n consecutive neighbor pairs.  This is the
   definition of OEIS A255662.  The algorithm is a simple dynamic
   programming over the last element and the number of downsteps counted
   in each of the two windows. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* State: cur[prev_value][c1][c2] where
       prev_value : previous element (0..3)
       c1        : number of downsteps seen in the first window so far (0..2)
       c2        : number of downsteps seen in the second window so far (0..2)
    */
    seq_t cur[4][3][3] = {0};
    seq_t nxt[4][3][3] = {0};

    /* Initial state: first element can be any value, no pairs yet. */
    for (int v = 0; v < 4; ++v) cur[v][0][0] = 1;

    /* Number of pairs is n+1.  Pairs are indexed 0 .. n. */
    for (idx_t i = 0; i <= n; ++i) {
        /* Reset next layer */
        for (int v = 0; v < 4; ++v)
            for (int c1 = 0; c1 < 3; ++c1)
                for (int c2 = 0; c2 < 3; ++c2)
                    nxt[v][c1][c2] = 0;

        for (int prev = 0; prev < 4; ++prev)
            for (int c1 = 0; c1 < 3; ++c1)
                for (int c2 = 0; c2 < 3; ++c2) {
                    seq_t cnt = cur[prev][c1][c2];
                    if (cnt == 0) continue;
                    for (int curv = 0; curv < 4; ++curv) {
                        int nc1 = c1;
                        int nc2 = c2;
                        if (prev > curv) {
                            if (i <= n - 1) ++nc1;           /* first window */
                            if (i >= 1 && i <= n) ++nc2;     /* second window */
                        }
                        if (nc1 > 2 || nc2 > 2) continue;
                        nxt[curv][nc1][nc2] += cnt;
                    }
                }

        /* Move to next layer */
        for (int v = 0; v < 4; ++v)
            for (int c1 = 0; c1 < 3; ++c1)
                for (int c2 = 0; c2 < 3; ++c2)
                    cur[v][c1][c2] = nxt[v][c1][c2];
    }

    /* Sum over all ending states */
    seq_t result = 0;
    for (int v = 0; v < 4; ++v)
        for (int c1 = 0; c1 < 3; ++c1)
            for (int c2 = 0; c2 < 3; ++c2)
                result += cur[v][c1][c2];

    return result;
}
