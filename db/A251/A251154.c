/*
 * A251154  Number of (n+1)X(4+1) 0..2 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays whose entries are in {0,1,2} *
 *            and for every 2×2 subblock with diagonal entries (x0,x1) and *
 *            antidiagonal entries (y0,y1) we have min(x0,x1) ≤ |y0−y1|
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over row
 * configurations.  Each row configuration is a base‑3 number with
 * five digits (0,1,2), giving 3^5 = 243 states.  An adjacency matrix
 * A contains an entry A[s][t] = 1 iff a 2×2 subblock formed by row
 * s and row t is allowed.  The number of arrays of height n+1 is
 * 1ᵗ · Aⁿ · 1.  We pre‑compute the adjacency lists once and then
 * multiply a vector of length 243 by that adjacency matrix n times
 * using a pointer swap trick. */

#include <stdbool.h>
#include <stdlib.h>

#define STATE_COUNT 243          /* 3^5 */
#define NUM_COLS    5
#define NUM_TRANS   4            /* 2×2 subblocks per row pair */

/* Store base‑3 digits of each state: digits[state][col] */
static unsigned char state_digits[STATE_COUNT][NUM_COLS];

/* Adjacency lists: for each state, list of allowed next states */
static int allowed_next[STATE_COUNT][STATE_COUNT];
static int allowed_count[STATE_COUNT] = {0};

/* Flag to build adjacency only once */
static bool adjacency_built = false;

/* Precompute base‑3 representation of states */
static void build_state_digits(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        int tmp = s;
        for (int c = 0; c < NUM_COLS; ++c) {
            state_digits[s][c] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }
}

/* Check if transition from state s to state t is allowed */
static bool is_allowed(int s, int t)
{
    for (int c = 0; c < NUM_TRANS; ++c) {
        int sc  = state_digits[s][c];
        int tc  = state_digits[t][c];
        int sc1 = state_digits[s][c + 1];
        int tc1 = state_digits[t][c + 1];

        int min_diag = (sc <= tc1) ? sc : tc1;
        int diff = sc1 - tc;
        if (diff < 0) diff = -diff;

        if (min_diag > diff)
            return false;
    }
    return true;
}

/* Build adjacency lists once */
static void build_adjacency(void)
{
    if (adjacency_built)
        return;
    build_state_digits();
    for (int s = 0; s < STATE_COUNT; ++s) {
        for (int t = 0; t < STATE_COUNT; ++t) {
            if (is_allowed(s, t)) {
                allowed_next[s][allowed_count[s]++] = t;
            }
        }
    }
    adjacency_built = true;
}

/* Static arrays for vector multiplication; these survive across calls */
static seq_t curr[STATE_COUNT];
static seq_t next[STATE_COUNT];

/* Core function required by the judge */
seq_t a(idx_t n)
{
    build_adjacency();

    /* Pointers to the two working vectors */
    seq_t *src = curr;
    seq_t *dst = next;

    /* initialize source vector: first row can be any state */
    for (int i = 0; i < STATE_COUNT; ++i)
        src[i] = (seq_t)1;

    /* Apply n transitions (n steps) */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero destination array */
        for (int i = 0; i < STATE_COUNT; ++i)
            dst[i] = (seq_t)0;

        /* Transition from each source state to its allowed successors */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = src[s];
            int cnt = allowed_count[s];
            int *list = allowed_next[s];
            for (int j = 0; j < cnt; ++j) {
                int t = list[j];
                dst[t] += val;
            }
        }

        /* Swap source and destination pointers for next iteration */
        seq_t *tmp = src;
        src = dst;
        dst = tmp;
    }

    /* Sum all counts in the final source vector */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += src[i];

    return total;
}
