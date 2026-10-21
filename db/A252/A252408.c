/*
 * A252408  Number of (1+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 2 3 4 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 2 3 4 6 or 7.
 * https://oeis.org/A252408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3 × (n+2) arrays with entries 0..3 such that
 *            for every 3×3 subblock its three row sums and its main diagonal
 *            sum are in {2,3,4,6,7} and its three column sums and its
 *            antidiagonal sum are not in that set
 * Range:     u8 none, u16 n <= 10, u32 n <= 29, u64 n <= 66, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds a transition table between consecutive column
   pairs.  There are 4³=64 possible columns (encoded base‑4).  A state is
   a pair of two consecutive columns (12 bits).  Transition from
   (c0,c1) to (c1,c2) is allowed iff the 3×3 block [c0,c1,c2] satisfies
   the constraints.  After n transitions we have n+2 columns (the
   required width).  The total number of configurations is the sum of all
   reachable states after n steps. */
#include <stdbool.h>
#include <stddef.h>

#define STATE_COUNT 4096                /* 4³ × 4³ = 64² */

/* Allowed set {2,3,4,6,7} encoded up to index 8.  0 means not allowed. */
static const unsigned char ALLOWED[9] = {
    0, 0, 1, 1, 1, 0, 1, 1, 0
};

static inline bool in_allowed(int sum)
{
    return sum <= 8 && ALLOWED[sum];
}

/* Decode a single column (base‑4) into r[0..2] = values in the three rows. */
static inline void decode_col(int col, int r[3])
{
    r[0] =  col       & 3;
    r[1] = (col >> 2) & 3;
    r[2] = (col >> 4) & 3;
}

/* Transition table.  For each state we keep the list of next states
   and its length. */
static unsigned short next_state[STATE_COUNT][64];
static unsigned char  next_len[STATE_COUNT];
static bool           transitions_inited = false;

/* Initialize the transition table once. */
static void init_transitions(void)
{
    if (transitions_inited)
        return;

    for (int v0 = 0; v0 < 64; ++v0) {
        int r0[3];
        decode_col(v0, r0);
        for (int v1 = 0; v1 < 64; ++v1) {
            int r1[3];
            decode_col(v1, r1);
            int state = (v0 << 6) | v1;
            unsigned char cnt = 0;
            for (int v2 = 0; v2 < 64; ++v2) {
                int r2[3];
                decode_col(v2, r2);

                bool ok = true;
                /* Row sums (each row over three columns). */
                for (int i = 0; i < 3 && ok; ++i) {
                    int sum = r0[i] + r1[i] + r2[i];
                    if (!in_allowed(sum))
                        ok = false;
                }
                /* Main diagonal. */
                if (ok) {
                    int md = r0[0] + r1[1] + r2[2];
                    if (!in_allowed(md))
                        ok = false;
                }
                /* Column sums (each column over three rows) must NOT be allowed. */
                if (ok) {
                    int c0 = r0[0] + r0[1] + r0[2];
                    int c1 = r1[0] + r1[1] + r1[2];
                    int c2 = r2[0] + r2[1] + r2[2];
                    if (in_allowed(c0) || in_allowed(c1) || in_allowed(c2))
                        ok = false;
                }
                /* Antidiagonal must NOT be allowed. */
                if (ok) {
                    int ad = r2[0] + r1[1] + r0[2];
                    if (in_allowed(ad))
                        ok = false;
                }

                if (ok) {
                    unsigned short nxt = (v1 << 6) | v2;
                    next_state[state][cnt++] = nxt;
                }
            }
            next_len[state] = cnt;
        }
    }
    transitions_inited = true;
}

/* DP buffers.  Two arrays are swapped each step. */
static seq_t dp[STATE_COUNT];
static seq_t ndp[STATE_COUNT];

/* Main function.  Returns a( n ) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_transitions();

    static seq_t *cur = dp, *nxt = ndp;

    /* Start with all possible initial column pairs. */
    for (int i = 0; i < STATE_COUNT; ++i)
        cur[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next buffer. */
        for (int i = 0; i < STATE_COUNT; ++i)
            nxt[i] = (seq_t)0;

        /* Apply transitions. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0)
                continue;
            unsigned char len = next_len[s];
            unsigned short *edges = next_state[s];
            for (unsigned char e = 0; e < len; ++e)
                nxt[edges[e]] += val;
        }

        /* Swap for next iteration. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all final states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += cur[i];

    return total;
}
