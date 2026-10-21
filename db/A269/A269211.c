/*
 * A269211  Number of nX5 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×5 arrays over {0,1,2,3} with exactly one *
 *            horizontally, diagonally or antidiagonally adjacent pair summing
 *            to 3
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses a dynamic‑programming recurrence over all admissible
 * 5‑cell rows.  Patterns containing more than one horizontal pair that
 * sums to 3 are discarded, because adding another such pair while extending
 * the array would immediately violate the “exactly one” condition.
 * For each row we keep two states:
 *   dp0[i] – number of ways to reach the row pattern i (of the compact
 *            pattern set) with zero such pairs so far;
 *   dp1[i] – number of ways with exactly one such pair so far.
 * Transitions between rows depend only on the pattern in the previous
 * row and are pre‑computed once.  The implementation avoids memcpy on
 * seq_t objects to stay well‑defined for arbitrary‑precision C++ builds. */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PAT 1024 /* 4⁵ = 1024 possible rows */

/* compact pattern representation (values 0..3) */
static unsigned char pat_digits[MAX_PAT][5];
static unsigned char pat_hsum[MAX_PAT];   /* horizontal sum==3 pairs in the row (0 or 1) */
static int pat_idx[MAX_PAT];             /* mapping from original 4⁵ pattern -> compact index */
static int P;                            /* number of valid compact patterns (hsum ≤1) */

/* transition lists: inc0 – no new match, inc1 – exactly one new match */
static int inc0list[MAX_PAT][MAX_PAT];
static int inc0len[MAX_PAT];
static int inc1list[MAX_PAT][MAX_PAT];
static int inc1len[MAX_PAT];

static bool init_done = false;

/* ------------------------------------------------------------------------- */
/* Build all patterns with at most one horizontal pair summing to 3.        */
static void init_patterns(void)
{
    unsigned char hsum0[MAX_PAT];
    unsigned char digs0[MAX_PAT][5];

    /* enumerate all 4⁵ patterns */
    for (int i = 0; i < MAX_PAT; ++i) {
        int v = i;
        for (int j = 0; j < 5; ++j) {
            digs0[i][j] = (unsigned char)(v & 3);
            v >>= 2;
        }
        unsigned int cnt = 0;
        for (int j = 0; j < 4; ++j) {
            if (digs0[i][j] + digs0[i][j + 1] == 3)
                ++cnt;
        }
        hsum0[i] = (unsigned char)cnt;
    }

    /* keep only patterns with ≤1 horizontal pair */
    P = 0;
    for (int i = 0; i < MAX_PAT; ++i) {
        if (hsum0[i] <= 1) {
            pat_idx[i] = P;
            for (int j = 0; j < 5; ++j)
                pat_digits[P][j] = digs0[i][j];
            pat_hsum[P] = hsum0[i];
            ++P;
        } else
            pat_idx[i] = -1;
    }
}

/* ------------------------------------------------------------------------- */
/* Pre‑compute transition lists between patterns.                            */
static void init_transitions(void)
{
    /* initialise lengths */
    for (int i = 0; i < P; ++i) {
        inc0len[i] = 0;
        inc1len[i] = 0;
    }

    /* for every pair of compact patterns, compute number of new matches */
    for (int p = 0; p < P; ++p) {
        for (int q = 0; q < P; ++q) {
            int dsum = 0;
            /* diagonal / anti‑diagonal adjacencies */
            for (int c = 0; c < 4; ++c) {
                if (pat_digits[p][c] + pat_digits[q][c + 1] == 3)
                    ++dsum;
                if (pat_digits[p][c + 1] + pat_digits[q][c] == 3)
                    ++dsum;
            }
            int inc = pat_hsum[q] + dsum;
            if (inc == 0)
                inc0list[p][inc0len[p]++] = q;
            else if (inc == 1)
                inc1list[p][inc1len[p]++] = q;
            /* inc > 1 transitions are discarded automatically */
        }
    }
}

/* ------------------------------------------------------------------------- */
/* Main function computing a(n).                                             */
seq_t a(idx_t n)
{
    if (!init_done) {
        init_done = true;
        init_patterns();
        init_transitions();
    }

    if (n < 1)
        return (seq_t)0; /* undefined behaviour for n < 1 */

    /* DP tables */
    seq_t dp0[MAX_PAT];
    seq_t dp1[MAX_PAT];
    seq_t next0[MAX_PAT];
    seq_t next1[MAX_PAT];

    seq_t zero = 0;
    for (int i = 0; i < P; ++i) {
        dp0[i] = zero;
        dp1[i] = zero;
    }

    /* base: first row */
    for (int i = 0; i < P; ++i) {
        if (pat_hsum[i] == 0)
            dp0[i] = 1;
        else /* pat_hsum[i] == 1 */
            dp1[i] = 1;
    }

    /* If only one row, return sum of dp1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < P; ++i)
            ans += dp1[i];
        return ans;
    }

    /* iterate rows 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        /* initialise next tables */
        for (int i = 0; i < P; ++i) {
            next0[i] = zero;
            next1[i] = zero;
        }

        for (int p = 0; p < P; ++p) {
            seq_t val0 = dp0[p];
            seq_t val1 = dp1[p];

            if (val0 != 0) {
                /* transitions that keep match count at 0 */
                for (int k = 0; k < inc0len[p]; ++k) {
                    int q = inc0list[p][k];
                    next0[q] += val0;
                }
                /* transitions that increase match count to 1 */
                for (int k = 0; k < inc1len[p]; ++k) {
                    int q = inc1list[p][k];
                    next1[q] += val0;
                }
            }

            if (val1 != 0) {
                /* from state with one match, only transitions that do not
                   create a second match are allowed */
                for (int k = 0; k < inc0len[p]; ++k) {
                    int q = inc0list[p][k];
                    next1[q] += val1;
                }
            }
        }

        /* move next → current using assignment (not memcpy) */
        for (int i = 0; i < P; ++i) {
            dp0[i] = next0[i];
            dp1[i] = next1[i];
        }
    }

    /* sum over all patterns in state with exactly one match */
    seq_t ans = 0;
    for (int i = 0; i < P; ++i)
        ans += dp1[i];

    return ans;
}
