/*
 * A282645  Number of nX6 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors.
 * https://oeis.org/A282645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×6 0..1 arrays with no 1 having more than one
 *  king‑move neighbor.  The function is computed by dynamic programming
 *  over rows.  Each DP state is a pair (prev_pattern, avail_mask).
 *  trans_next[state] holds all legal next states; only one transition
 *  table is built on the first call and reused for all subsequent calls.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PAT   64          /* 2^6 possible row patterns */
#define STATE (PAT * PAT) /* (prev_pattern, avail_mask) states */

static unsigned char   trans_cnt[STATE];
static unsigned short  trans_next[STATE][PAT];
static int            transitions_initialized = 0;

/* ------------------------------------------------------------------ */
/* Build the transition table only once                                 */
/* ------------------------------------------------------------------ */
static void init_transitions(void)
{
    for (int P = 0; P < PAT; ++P) {
        for (int A = 0; A < PAT; ++A) {
            unsigned int state_index = (P << 6) | A;
            unsigned char idx = 0;

            for (int C = 0; C < PAT; ++C) {
                bool ok = true;

                /* Check each 1 in the current row C: it may have at most
                 * one neighbor among left, right, up-left, up, up-right.
                 */
                for (int j = 0; j < 6 && ok; ++j) {
                    if (C & (1 << j)) {
                        int cnt = 0;
                        if (j > 0)     cnt += (C >> (j - 1)) & 1;
                        if (j < 5)     cnt += (C >> (j + 1)) & 1;
                        if (j > 0)     cnt += (P >> (j - 1)) & 1;
                        if (1)         cnt += (P >>  j) & 1;
                        if (j < 5)     cnt += (P >> (j + 1)) & 1;
                        if (cnt > 1)   ok = false;
                    }
                }
                if (!ok) continue;

                /* Check for each 1 in the previous row P whether
                 * its neighbour count from below respects
                 * the available mask A.  A bit = 0 means the cell
                 * already had a neighbour and cannot accept another.
                 * A bit = 1 means it has no neighbour yet and can
                 * have at most one from below.
                 */
                for (int j = 0; j < 6 && ok; ++j) {
                    if (P & (1 << j)) {
                        int cnt = 0;
                        if (j > 0)     cnt += (C >> (j - 1)) & 1;
                        if (1)         cnt += (C >>  j) & 1;
                        if (j < 5)     cnt += (C >> (j + 1)) & 1;
                        if (!(A & (1 << j))) {       /* already used */
                            if (cnt > 0) ok = false;
                        } else {                      /* still free */
                            if (cnt > 1) ok = false;
                        }
                    }
                }
                if (!ok) continue;

                /* Compute the new availability mask for row C.  A cell
                 * in C that currently has zero neighbours (from left,
                 * right, and the previous row) may accept one neighbour
                 * from the next row, so we set its bit to 1.
                 */
                int newA = 0;
                for (int j = 0; j < 6; ++j) {
                    if (C & (1 << j)) {
                        int cnt = 0;
                        if (j > 0)     cnt += (C >> (j - 1)) & 1;
                        if (j < 5)     cnt += (C >> (j + 1)) & 1;
                        if (j > 0)     cnt += (P >> (j - 1)) & 1;
                        if (1)         cnt += (P >>  j) & 1;
                        if (j < 5)     cnt += (P >> (j + 1)) & 1;
                        if (cnt == 0)  newA |= (1 << j);
                    }
                }

                unsigned int next_state = (C << 6) | newA;
                trans_next[state_index][idx++] = (unsigned short)next_state;
            }

            trans_cnt[state_index] = idx;
        }
    }

    transitions_initialized = 1;
}

/* ------------------------------------------------------------------ */
/* DP storage for one column of rows                                   */
/* ------------------------------------------------------------------ */
static seq_t dp[STATE];
static seq_t nxt[STATE];

/* ------------------------------------------------------------------ */
/* Public function                                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!transitions_initialized)
        init_transitions();

    /* initialise dp for zero rows */
    for (int i = 0; i < STATE; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1; /* previous row pattern 0, availability 0 */

    for (idx_t row = 0; row < n; ++row) {
        /* zero next array */
        for (int i = 0; i < STATE; ++i) nxt[i] = (seq_t)0;

        for (int state = 0; state < STATE; ++state) {
            seq_t val = dp[state];
            if (val == (seq_t)0) continue;
            unsigned char cnt = trans_cnt[state];
            for (unsigned char k = 0; k < cnt; ++k) {
                unsigned short ns = trans_next[state][k];
                nxt[ns] += val;
            }
        }

        /* swap dp and nxt */
        for (int i = 0; i < STATE; ++i) dp[i] = nxt[i];
    }

    /* sum over all terminal states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE; ++i) result += dp[i];
    return result;
}
