/*
 * A263435  Number of (n+1)X(2+1) 0..4 arrays with each row and column
 *          divisible by 13, read as a base-5 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   mod13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by DP over column remainder states.
   a(n) = number of (n+1)×3 arrays with digits 0..4 such that
          every row and every column, read as a base‑5 number
          (top/left most significant), is divisible by 13. */

#include <stdlib.h>
#include "../utils/mod13.h"

static const int PAT[10][3] = {
    {0,0,0}, {0,2,3}, {1,0,1}, {1,2,4},
    {2,0,2}, {2,3,0}, {3,0,3}, {3,3,1},
    {4,0,4}, {4,3,2}
};

static int TRANS[2197][10];
static int TRANS_INIT = 0;

seq_t a(idx_t n) {
    const int STATES = 2197;
    const int ROWS = (int)(n + 1);   /* number of rows = n+1 */

    /* lazy initialization of state transition table */
    if (!TRANS_INIT) {
        for (int s1 = 0; s1 < 13; ++s1) {
            for (int s2 = 0; s2 < 13; ++s2) {
                for (int s3 = 0; s3 < 13; ++s3) {
                    int idx = s1 * 169 + s2 * 13 + s3;
                    for (int p = 0; p < 10; ++p) {
                        int ns1 = (s1 * 5 + PAT[p][0]) % 13;
                        int ns2 = (s2 * 5 + PAT[p][1]) % 13;
                        int ns3 = (s3 * 5 + PAT[p][2]) % 13;
                        TRANS[idx][p] = ns1 * 169 + ns2 * 13 + ns3;
                    }
                }
            }
        }
        TRANS_INIT = 1;
    }

    /* dynamic allocation of DP arrays reused across calls */
    static seq_t *cur = NULL;
    static seq_t *next = NULL;
    static size_t cap = 0;
    if (cap < (size_t)STATES) {
        cur  = (seq_t *)realloc(cur,  STATES * sizeof(seq_t));
        next = (seq_t *)realloc(next, STATES * sizeof(seq_t));
        cap = STATES;
    }

    /* initialize state vectors */
    for (size_t i = 0; i < (size_t)STATES; ++i) {
        cur[i] = (seq_t)0;
        next[i] = (seq_t)0;
    }
    cur[0] = (seq_t)1;    /* start with zero remainders */

    /* DP over rows */
    for (int r = 0; r < ROWS; ++r) {
        for (size_t i = 0; i < (size_t)STATES; ++i)
            next[i] = (seq_t)0;

        for (size_t idx = 0; idx < (size_t)STATES; ++idx) {
            seq_t val = cur[idx];
            if (val == (seq_t)0) continue;
            for (int p = 0; p < 10; ++p) {
                size_t nidx = TRANS[idx][p];
                next[nidx] = next[nidx] + val;
            }
        }

        /* swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    return cur[0];   /* full column remainders zero after all rows */
}
