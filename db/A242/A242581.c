/*
 * A242581  Number of length n+3 0..5 arrays with no four elements in a row
 *          with pattern aabb (with a!=b) and new values 0..5 introduced in
 *          0..5 order.
 * https://oeis.org/A242581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over {0,1,2,3,4,5} in which
 *            new values are introduced in order 0..5 and no consecutive four
 *            elements form the pattern a a b b with a ≠ b. Construction:
 *            dynamic programming over the current maximum symbol used (0..5)
 *            and the last three values (represented using a...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_K      6
#define LAST_D     7                      /* 0..6 inclusive, 6 is sentinel */
#define LAST_STATES (LAST_D * LAST_D * LAST_D)    /* 7³ = 343 */
#define STATE_COUNT   (MAX_K * LAST_STATES)        /* 6 * 343 = 2058 */

seq_t a(idx_t n)
{
    idx_t len = n + 3;                 /* length of the arrays */

    /* static arrays so we allocate them only once */
    static seq_t cur[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];

    /* initialise arrays to zero for this call */
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
    }

    /* sentinel value for “not yet defined” in the last three positions */
    const int sentinel = 6;

    /* start with the first element forced to 0:
       maximum used = 0, last three = {6,6,0} */
    const int init_last = sentinel * LAST_D * LAST_D + sentinel * LAST_D + 0;
    cur[0 * LAST_STATES + init_last] = 1;

    /* iterate over positions 2 .. len */
    for (idx_t step = 2; step <= len; ++step) {
        /* zero the next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;

        for (int maxk = 0; maxk < MAX_K; ++maxk) {
            size_t base = (size_t)maxk * LAST_STATES;
            for (int a = 0; a < LAST_D; ++a) {
                for (int b = 0; b < LAST_D; ++b) {
                    for (int c = 0; c < LAST_D; ++c) {
                        size_t idx_prev = base
                                          + (size_t)a * LAST_D * LAST_D
                                          + (size_t)b * LAST_D
                                          + (size_t)c;
                        seq_t val = cur[idx_prev];
                        if (val == 0) continue;

                        int maxx = maxk + 1;
                        if (maxx > 5) maxx = 5;

                        for (int x = 0; x <= maxx; ++x) {
                            int new_k = (x == maxk + 1) ? maxk + 1 : maxk;

                            /* forbid the forbidden pattern a a b b (a!=b) */
                            if (a != sentinel && b != sentinel && c != sentinel &&
                                a == b && c == x && a != c) {
                                continue;
                            }

                            int na = b;
                            int nb = c;
                            int nc = x;

                            size_t idx_next = (size_t)new_k * LAST_STATES
                                              + (size_t)na * LAST_D * LAST_D
                                              + (size_t)nb * LAST_D
                                              + (size_t)nc;
                            nxt[idx_next] += val;
                        }
                    }
                }
            }
        }

        /* prepare for next step */
        for (size_t i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    /* sum all final states to obtain the answer */
    seq_t ans = 0;
    for (size_t i = 0; i < STATE_COUNT; ++i) ans += cur[i];
    return ans;
}
