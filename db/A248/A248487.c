/*
 * A248487  Number of length n+5 0..6 arrays with some three disjoint pairs in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 29, u64 n <= 75, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a finite‑state automaton with 7⁵ states; the number of
   length (n+5) arrays over {0,…,6} in which every consecutive six
   terms contain a disjoint triple of pairs with equal sums is
   counted by dynamic programming over the transition table. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define ALPHA 7
#define STATE_COUNT 16807          /* 7⁵ */

typedef uint8_t u8;

/* All 15 perfect matchings of {0,…,5}
   Each row contains the first elements of the three pairs,
   the corresponding second elements are in match_j. */
static const int match_i[15][3] = {
    {0,2,4}, {0,2,3}, {0,2,3},
    {0,1,4}, {0,1,3}, {0,1,3},
    {0,1,4}, {0,1,2}, {0,1,2},
    {0,1,3}, {0,1,2}, {0,1,2},
    {0,1,3}, {0,1,2}, {0,1,2}
};

static const int match_j[15][3] = {
    {1,3,5}, {1,4,5}, {1,5,4},
    {2,3,5}, {2,4,5}, {2,5,4},
    {3,2,5}, {3,4,5}, {3,5,4},
    {4,2,5}, {4,3,5}, {4,5,3},
    {5,2,4}, {5,3,4}, {5,4,3}
};

/* Transition table: next_state[s][v] = new state index
   when the current state s (encoded as 5‑tuple) is extended by value v.
   A value of -1 indicates the extended 6‑tuple is invalid. */
static int16_t next_state[STATE_COUNT][ALPHA];
static bool initialized = false;

/* Test whether a 6‑tuple admits some perfect matching with equal pair sums. */
static bool ok6(const u8 a[6])
{
    int sum = a[0]+a[1]+a[2]+a[3]+a[4]+a[5];
    if (sum % 3) return false;
    int S = sum / 3;
    for (int m = 0; m < 15; ++m) {
        int s1 = a[match_i[m][0]] + a[match_j[m][0]];
        if (s1 != S) continue;
        int s2 = a[match_i[m][1]] + a[match_j[m][1]];
        if (s2 != S) continue;
        int s3 = a[match_i[m][2]] + a[match_j[m][2]];
        if (s3 == S) return true;
    }
    return false;
}

/* Build the transition table once. */
static void build_transition(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        u8 cur[5];
        int tmp = s;
        for (int i = 4; i >= 0; --i) {
            cur[i] = tmp % ALPHA;
            tmp /= ALPHA;
        }
        for (int v = 0; v < ALPHA; ++v) {
            u8 tup6[6];
            for (int i = 0; i < 5; ++i) tup6[i] = cur[i];
            tup6[5] = (u8)v;
            if (!ok6(tup6)) {
                next_state[s][v] = -1;
                continue;
            }
            int enc = 0;
            for (int i = 1; i < 5; ++i) {
                enc = enc * ALPHA + cur[i];
            }
            enc = enc * ALPHA + v;
            next_state[s][v] = (int16_t)enc;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_transition();
        initialized = true;
    }

    static seq_t dp[STATE_COUNT];
    static seq_t np[STATE_COUNT];

    /* initialise to 1 for all starting 5‑tuples */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 1;

    seq_t *cur = dp;
    seq_t *next = np;

    for (idx_t step = 0; step < n; ++step) {
        /* clear next */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            int16_t *row = next_state[s];
            for (int v = 0; v < ALPHA; ++v) {
                int t = row[v];
                if (t >= 0) next[t] += val;
            }
        }
        /* swap */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += cur[i];
    return total;
}
