/*
 * A247400  Number of length n+4 0..4 arrays with no disjoint pairs in any
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 47, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length n+4 arrays over {0,1,2,3,4} with no disjoint pairs
   in any consecutive five terms having the same sum.  The algorithm
   uses dynamic programming over the last four entries of the array. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Alphabet size and state space (5^4 = 625) */
#define BASE      5
#define STATE_DIM 625

/* Encode a 4‑tuple (a,b,c,d), 0 ≤ a,b,c,d ≤ 4, into a unique index 0…624. */
static inline int encode4(int a, int b, int c, int d)
{
    return ((a * BASE + b) * BASE + c) * BASE + d;
}

/* Pre‑computed indices of the 10 unordered pairs of positions in a 5‑tuple. */
static const int pair_idx[10][2] = {
    {0,1},{0,2},{0,3},{0,4},
    {1,2},{1,3},{1,4},
    {2,3},{2,4},
    {3,4}
};

/* Check whether a 5‑tuple (a,b,c,d,e) satisfies the “no equal disjoint pair
   sums” condition.  Returns true if the window is valid. */
static bool window_valid(int a, int b, int c, int d, int e)
{
    int val[5] = {a,b,c,d,e};
    for (int p1 = 0; p1 < 10; ++p1)
        for (int p2 = p1 + 1; p2 < 10; ++p2) {
            int i1 = pair_idx[p1][0], j1 = pair_idx[p1][1];
            int i2 = pair_idx[p2][0], j2 = pair_idx[p2][1];
            /* ignore overlapping pairs */
            if (i1==i2 || i1==j2 || j1==i2 || j1==j2) continue;
            if (val[i1] + val[j1] == val[i2] + val[j2]) return false;
        }
    return true;
}

/* Transition tables: for every 4‑tuple state and every possible next digit,
   whether the resulting 5‑tuple is valid, and the index of the new state
   (last four digits).  These are built once. */
static bool tbl_valid[STATE_DIM][BASE];
static int  tbl_next[STATE_DIM][BASE];
static bool tbl_initialized = false;

/* Decode a state index into a 4‑tuple. */
static inline void decode4(int idx, int out[4])
{
    out[3] = idx % BASE; idx /= BASE;
    out[2] = idx % BASE; idx /= BASE;
    out[1] = idx % BASE; idx /= BASE;
    out[0] = idx;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* initialize the transition tables once */
    if (!tbl_initialized) {
        int qu[4];
        for (int i = 0; i < STATE_DIM; ++i) {
            decode4(i, qu);
            for (int e = 0; e < BASE; ++e) {
                tbl_valid[i][e] = window_valid(qu[0], qu[1], qu[2], qu[3], e);
                if (tbl_valid[i][e])
                    tbl_next[i][e] = encode4(qu[1], qu[2], qu[3], e);
            }
        }
        tbl_initialized = true;
    }

    /* total length of arrays: n + 4 */
    const size_t N = (size_t)n + 4;

    seq_t dp[STATE_DIM];
    seq_t nextdp[STATE_DIM];

    /* length 4: all 4‑tuples are allowed → count = 1 each */
    for (int i = 0; i < STATE_DIM; ++i) dp[i] = (seq_t)1;

    /* Extend the array length from 5 up to N */
    for (size_t len = 5; len <= N; ++len) {
        for (int i = 0; i < STATE_DIM; ++i) nextdp[i] = (seq_t)0;

        for (int i = 0; i < STATE_DIM; ++i) {
            seq_t cur = dp[i];
            if (cur == 0) continue;
            for (int e = 0; e < BASE; ++e) {
                if (!tbl_valid[i][e]) continue;
                int new_idx = tbl_next[i][e];
                nextdp[new_idx] += cur;
            }
        }
        /* swap dp and nextdp */
        for (int i = 0; i < STATE_DIM; ++i) dp[i] = nextdp[i];
    }

    /* sum over all ending states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_DIM; ++i) total += dp[i];
    return total;
}
