/*
 * A250921  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having its
 *          minimum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) counts (n+1)x3 0..2 arrays where for every 2x2 subblock
   the minimum of the main diagonal is at least the minimum of the anti-diagonal.
   The problem is reduced to walks on a directed graph of the 27 possible
   3-column rows.  The adjacency matrix is precomputed once; dynamic
   programming over n transitions yields the answer. */

#include <stdbool.h>

static uint8_t rows[27][3];
static bool trans[27][27];
static bool init_done = false;

static void init_trans(void)
{
    /* Encode each row (3 digits in base 3). */
    for (idx_t i = 0; i < 27; ++i) {
        rows[i][0] = (i / 9) % 3;
        rows[i][1] = (i / 3) % 3;
        rows[i][2] = i % 3;
    }

    for (idx_t i = 0; i < 27; ++i) {
        uint8_t a0 = rows[i][0], a1 = rows[i][1], a2 = rows[i][2];
        for (idx_t j = 0; j < 27; ++j) {
            uint8_t b0 =   rows[j][0];
            uint8_t b1 =   rows[j][1];
            uint8_t b2 =   rows[j][2];

            /* Check first 2x2 subblock (columns 0-1). */
            uint8_t min_diag1  = a0 < b1 ? a0 : b1;
            uint8_t min_anti1 = a1 < b0 ? a1 : b0;
            bool cond1 = (min_diag1 >= min_anti1);

            /* Check second 2x2 subblock (columns 1-2). */
            uint8_t min_diag2  = a1 < b2 ? a1 : b2;
            uint8_t min_anti2 = a2 < b1 ? a2 : b1;
            bool cond2 = (min_diag2 >= min_anti2);

            trans[i][j] = cond1 && cond2;
        }
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        init_trans();
        init_done = true;
    }

    seq_t dp[27], next[27];
    for (idx_t i = 0; i < 27; ++i) dp[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (idx_t j = 0; j < 27; ++j) next[j] = 0;
        for (idx_t i = 0; i < 27; ++i) {
            seq_t cur = dp[i];
            for (idx_t j = 0; j < 27; ++j) {
                if (trans[i][j]) next[j] += cur;
            }
        }
        for (idx_t i = 0; i < 27; ++i) dp[i] = next[i];
    }

    seq_t res = 0;
    for (idx_t i = 0; i < 27; ++i) res += dp[i];
    return res;
}
