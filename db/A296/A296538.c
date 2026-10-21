/*
 * A296538  Number of nX5 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A296538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: transfer-matrix method for n×5 0..1 arrays where each 1 has 2 or 4 adjacent 1s
   (adjacent horizontally, vertically or antidiagonally). */

#include <stdint.h>
#include <stddef.h>

#define COLS 5
#define STATES (1U << COLS)

/* Precomputed transition masks and final-row validity. */
static uint32_t trans_masks[STATES][STATES];
static unsigned char final_valid[STATES][STATES];

/* Check whether the middle row 'prev' satisfies the degree condition
   given the row above 'prevprev' and the row below 'cur'. */
static int check_row(unsigned int prevprev, unsigned int prev, unsigned int cur)
{
    for (int c = 0; c < COLS; ++c) {
        unsigned int bit = 1U << c;
        if (prev & bit) {
            unsigned int deg = 0;
            /* left */
            if (c > 0 && (prev & (1U << (c - 1)))) ++deg;
            /* right */
            if (c < COLS - 1 && (prev & (1U << (c + 1)))) ++deg;
            /* up */
            if (prevprev & bit) ++deg;
            /* up-right */
            if (c < COLS - 1 && (prevprev & (1U << (c + 1)))) ++deg;
            /* down */
            if (cur & bit) ++deg;
            /* down-left */
            if (c > 0 && (cur & (1U << (c - 1)))) ++deg;
            if (!(deg == 2 || deg == 4))
                return 0;
        }
    }
    return 1;
}

/* One-time initialization of transition data. */
static void init_transitions(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    for (unsigned int pp = 0; pp < STATES; ++pp) {
        for (unsigned int p = 0; p < STATES; ++p) {
            uint32_t m = 0;
            unsigned char final_flag = 0;
            for (unsigned int c = 0; c < STATES; ++c) {
                if (check_row(pp, p, c))
                    m |= (1U << c);
                if (c == 0)
                    final_flag = check_row(pp, p, 0);
            }
            trans_masks[pp][p] = m;
            final_valid[pp][p] = final_flag;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_transitions();
    if (n == 0) return 0; /* undefined, but safe */

    /* DP arrays: dp[prevprev][prev] */
    seq_t dp[STATES][STATES];
    seq_t next[STATES][STATES];
    /* initialise dp */
    for (int i = 0; i < STATES; ++i)
        for (int j = 0; j < STATES; ++j)
            dp[i][j] = 0;
    for (int mask = 0; mask < STATES; ++mask)
        dp[0][mask] = 1;

    /* iterate rows */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                next[i][j] = 0;
        for (int pp = 0; pp < STATES; ++pp) {
            for (int p = 0; p < STATES; ++p) {
                seq_t cnt = dp[pp][p];
                if (cnt == 0) continue;
                uint32_t mask = trans_masks[pp][p];
                for (int c = 0; c < STATES; ++c)
                    if (mask & (1U << c))
                        next[p][c] += cnt;
            }
        }
        /* copy next to dp */
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum over final states */
    seq_t ans = 0;
    for (int pp = 0; pp < STATES; ++pp) {
        for (int p = 0; p < STATES; ++p) {
            seq_t cnt = dp[pp][p];
            if (cnt == 0) continue;
            if (final_valid[pp][p])
                ans += cnt;
        }
    }
    return ans;
}
