/*
 * A254547  Number of (n+2) X (2+2) 0..1 arrays with every 3 X 3 subblock
 *          diagonal sum minus antidiagonal minimum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 13, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   addseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A254547: Number of (n+2)×4 binary arrays whose 3×3 block
 * “diagonal sum minus antidiagonal minimum” forms a
 * nondecreasing array in the row and column directions.
 *
 * The function implements a DP over row patterns:
 *   dp[r1][r2][s] = number of partial arrays ending with
 *   rows r1,r2 where the last produced 3×3 block
 *   (starting at the row two steps above) has
 *   differences coded by s.
 *   s is a pair of differences (left,right); 5 possible
 *   values are -1..3, encoded as 0..4.  A sentinel
 *   value 25 indicates that no block has been produced yet.
 *
 * Complexity: O(n·16^3·25) ~ 1.3×10^5 elementary operations
 * per n, with small memory (≈6 kB).  Works for n≤10 000 in
 * well under 2 s.  All arithmetic is performed in seq_t
 * (which may be arbitrary precision). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/addseq.h"

/* number of 4‑bit row patterns */
#define PAT 16
/* number of encoded difference pairs (5×5) plus a sentinel */
#define SCOUNT 26
/* sentinel for “no previous diff yet” */
#define NO_DIFF 25

/* pre‑computed bits of each row pattern (columns 1‑4, 1‑indexed) */
static unsigned char bits_of_row[PAT][5];

/* initialise bits_of_row lazily */
static void init_bits(void)
{
    static bool inited;
    if (inited) return;
    for (int p = 0; p < PAT; ++p)
        for (int col = 1; col <= 4; ++col)
            bits_of_row[p][col] = (p >> (col - 1)) & 1u;
    inited = true;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    init_bits();

    /* a diff value is from -1 to 3, encoded as 0..4 by adding 1 */
    const int DIFF_MIN = -1;
    const int DIFF_MAX =  3;
    const int DIFF_STEPS = DIFF_MAX - DIFF_MIN + 1;  /* 5 */

    /* total number of rows in the array */
    const idx_t N = n + 2;

    /* DP arrays */
    static seq_t dp[PAT][PAT][SCOUNT];
    static seq_t dp_next[PAT][PAT][SCOUNT];

    /* clear dp arrays */
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            for (int k = 0; k < SCOUNT; ++k)
                dp[i][j][k] = dp_next[i][j][k] = 0;

    /* initialise states before any 3×3 block is formed */
    for (int r1 = 0; r1 < PAT; ++r1)
        for (int r2 = 0; r2 < PAT; ++r2)
            dp[r1][r2][NO_DIFF] = 1;

    /* iterate over rows that allow a 3×3 block */
    for (idx_t row = 3; row <= N; ++row)
    {
        /* clear dp_next */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                for (int k = 0; k < SCOUNT; ++k)
                    dp_next[i][j][k] = 0;

        for (int r1 = 0; r1 < PAT; ++r1)
            for (int r2 = 0; r2 < PAT; ++r2)
                for (int s = 0; s < SCOUNT; ++s)
                {
                    seq_t cur = dp[r1][r2][s];
                    if (!cur) continue;

                    /* try all possible next rows r3 */
                    for (int r3 = 0; r3 < PAT; ++r3)
                    {
                        /* compute left and right diffs for the new 3×3 block
                           (rows r1,r2,r3) starting at columns 1 and 2 */
                        int diag_left  = bits_of_row[r1][1]
                                        + bits_of_row[r2][2]
                                        + bits_of_row[r3][3];
                        int diag_right = bits_of_row[r1][2]
                                        + bits_of_row[r2][3]
                                        + bits_of_row[r3][4];

                        int anti_left  = bits_of_row[r1][3];
                        int anti_mid   = bits_of_row[r2][2];
                        int anti_bot   = bits_of_row[r3][1];
                        int anti_min_left = anti_left;
                        if (anti_mid < anti_min_left) anti_min_left = anti_mid;
                        if (anti_bot < anti_min_left) anti_min_left = anti_bot;

                        anti_left = anti_min_left;

                        int anti_left2  = bits_of_row[r1][4];
                        int anti_mid2   = bits_of_row[r2][3];
                        int anti_bot2   = bits_of_row[r3][2];
                        int anti_min_right = anti_left2;
                        if (anti_mid2 < anti_min_right) anti_min_right = anti_mid2;
                        if (anti_bot2 < anti_min_right) anti_min_right = anti_bot2;

                        anti_left2 = anti_min_right;

                        int diff_left  = diag_left  - anti_left;
                        int diff_right = diag_right - anti_left2;

                        /* horizontal monotonicity */
                        if (diff_left > diff_right) continue;

                        int idx_left  = diff_left  + 1;
                        int idx_right = diff_right + 1;
                        if (idx_left < 0 || idx_left >= DIFF_STEPS) continue;
                        if (idx_right < 0 || idx_right >= DIFF_STEPS) continue;

                        if (s != NO_DIFF) {
                            int prev_left_idx  = s / DIFF_STEPS;
                            int prev_right_idx = s % DIFF_STEPS;
                            int prev_left  = prev_left_idx  + DIFF_MIN;
                            int prev_right = prev_right_idx + DIFF_MIN;

                            /* vertical monotonicity */
                            if (diff_left  < prev_left)  continue;
                            if (diff_right < prev_right) continue;
                            /* anti‑diagonal (NE→SW) */
                            if (prev_right > diff_left) continue;
                        }

                        int ns = idx_left * DIFF_STEPS + idx_right;  /* 0..24 */

                        dp_next[r2][r3][ns] = addseq(dp_next[r2][r3][ns], cur);
                    }
                }

        /* move dp_next to dp for next iteration */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                for (int k = 0; k < SCOUNT; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* sum all states after processing last row */
    seq_t result = 0;
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            for (int k = 0; k < SCOUNT; ++k)
                result = addseq(result, dp[i][j][k]);

    return result;
}
