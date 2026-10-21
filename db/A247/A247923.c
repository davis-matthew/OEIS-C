/*
 * A247923  Number of length n+4 0..4 arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes a(n) for OEIS sequence A247923.
    a(n) = number of length n+4 arrays with elements in {0,1,2,3,4}
           such that every consecutive block of 5 terms contains
           two disjoint pairs having equal sum.
    The computation is done by dynamic programming on a state
    consisting of the last 4 elements (5^4=625 states).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* 4‑digit state: base‑5 encoding */
#define STATE_COUNT 625
#define ALPHA 5
#define MAX_TRANS 5            /* at most 5 outgoing digits */

/* transition tables (computed once) */
static int  trans_next[STATE_COUNT][MAX_TRANS];
static int  trans_cnt [STATE_COUNT];
static bool trans_inited = false;

/* all unordered pairs of indices (0..4) */
static const int pair_idx[10][2] = {
    {0,1},{0,2},{0,3},{0,4},{1,2},
    {1,3},{1,4},{2,3},{2,4},{3,4}
};

/* check whether a window of five digits satisfies the condition */
static bool window_valid(const int w[5])
{
    for (int p = 0; p < 10; ++p) {
        int sum1 = w[pair_idx[p][0]] + w[pair_idx[p][1]];
        int mask1 = (1 << pair_idx[p][0]) | (1 << pair_idx[p][1]);
        for (int q = p + 1; q < 10; ++q) {
            int mask2 = (1 << pair_idx[q][0]) | (1 << pair_idx[q][1]);
            if (mask1 & mask2)            /* pairs overlap */
                continue;
            int sum2 = w[pair_idx[q][0]] + w[pair_idx[q][1]];
            if (sum1 == sum2)
                return true;
        }
    }
    return false;
}

/* build transition tables on first use */
static void init_transitions(void)
{
    if (trans_inited)
        return;
    for (int s = 0; s < STATE_COUNT; ++s) {
        int a = (s / 125) % ALPHA;
        int b = (s / 25)  % ALPHA;
        int c = (s / 5)   % ALPHA;
        int d = s % ALPHA;
        trans_cnt[s] = 0;
        for (int e = 0; e < ALPHA; ++e) {
            int w[5] = {a, b, c, d, e};
            if (window_valid(w)) {
                trans_next[s][trans_cnt[s]] = ((b * 125) + (c * 25) + (d * 5) + e);
                ++trans_cnt[s];
            }
        }
    }
    trans_inited = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_transitions();            /* ensure transition tables exist */

    /* DP arrays: counts of sequences of current length ending in each state */
    seq_t fPrev[STATE_COUNT];
    seq_t fNext[STATE_COUNT];
    /* initial length = 4: any 4‑tuple is allowed, each with count 1 */
    for (int i = 0; i < STATE_COUNT; ++i)
        fPrev[i] = 1;

    /* perform n steps to reach length n+4 */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i)
            fNext[i] = 0;                 /* reset next counts */

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = fPrev[s];
            if (cnt == 0)
                continue;
            for (int t = 0; t < trans_cnt[s]; ++t) {
                int ns = trans_next[s][t];
                fNext[ns] = fNext[ns] + cnt;
            }
        }
        /* copy fNext -> fPrev for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i)
            fPrev[i] = fNext[i];
    }

    /* sum over all states to obtain total number of sequences of length n+4 */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total = total + fPrev[i];

    return total;
}
