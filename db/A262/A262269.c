/*
 * A262269  Number of (n+2)X(3+2) 0..1 arrays with each row and column
 *          divisible by 5, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×5 binary arrays whose every row read as a
 *            5‑bit integer and every column read as an (n+2)-bit integer are
 *            divisible by 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* 5‑bit row patterns whose integer value is divisible by 5 */
static const unsigned short PAT_BITS[7][5] = {
    {0,0,0,0,0},   /* 0   = 00000 */
    {0,0,1,0,1},   /* 5   = 00101 */
    {0,1,0,1,0},   /* 10  = 01010 */
    {0,1,1,1,1},   /* 15  = 01111 */
    {1,0,1,0,0},   /* 20  = 10100 */
    {1,1,0,0,1},   /* 25  = 11001 */
    {1,1,1,1,0}    /* 30  = 11110 */
};

/* number of DP states: each of 5 columns has a remainder 0..4 */
#define STATES 3125         /* 5^5 */

/* State encoding: s = r0 + r1*5 + r2*5^2 + r3*5^3 + r4*5^4   (0 ≤ rj < 5) */
static const unsigned short POW5S[5] = {1,5,25,125,625};

/* transition[p][s] = new state after applying pattern p to state s */
static unsigned short TRANS[7][STATES];   /* filled on first call */
static int TRANS_INIT = 0;

/* DP buffers kept across calls to avoid reallocations */
static seq_t *DP[2] = {NULL, NULL};
static int DP_INIT = 0;

seq_t a(idx_t n)
{
    const idx_t rows = n + 2;                /* matrix has (n+2) rows, 5 cols */
    /* initialise transition table once */
    if (!TRANS_INIT) {
        for (int ss = 0; ss < STATES; ++ss) {
            int rem[5];
            int tmp = ss;
            for (int j = 0; j < 5; ++j) {    /* decode base‑5 digits */
                rem[j] = tmp % 5;
                tmp   /= 5;
            }
            for (int p = 0; p < 7; ++p) {
                int newrem[5];
                for (int j = 0; j < 5; ++j)
                    newrem[j] = (rem[j] * 2 + PAT_BITS[p][j]) % 5;
                unsigned short ns =   newrem[0]
                                   + newrem[1]*POW5S[1]
                                   + newrem[2]*POW5S[2]
                                   + newrem[3]*POW5S[3]
                                   + newrem[4]*POW5S[4];
                TRANS[p][ss] = ns;
            }
        }
        TRANS_INIT = 1;
    }

    /* initialise DP buffers once */
    if (!DP_INIT) {
        size_t sz = (size_t)STATES * sizeof(seq_t);
        DP[0] = (seq_t*)malloc(sz);
        DP[1] = (seq_t*)malloc(sz);
        DP_INIT = 1;
    }

    /* alias for easier handling */
    seq_t *cur  = DP[0];
    seq_t *next = DP[1];

    /* clear current DP */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;
    cur[0] = 1;          /* empty prefix has remainder 0 in all columns */

    for (idx_t r = 0; r < rows; ++r) {
        /* clear next DP */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;

            /* try all 7 allowed row patterns */
            for (int p = 0; p < 7; ++p) {
                size_t ns = TRANS[p][s];
                next[ns] += val;
            }
        }
        /* swap pointers for the next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* final state must have all column remainders 0 */
    return cur[0];
}
