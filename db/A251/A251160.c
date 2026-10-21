/*
 * A251160  Number of (n+1)X(1+1) 0..3 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 matrices over {0,1,2,3} such that for
 *            every adjacent pair of columns the 2×2 subblock satisfies
 *            max(diagonal) ≤ |difference of antidiagonal|. Computed by a
 *            simple dynamic‑programming over 16 column states
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 16 possible column states: (top, bottom) in base‑4
       state = 4*top + bottom.  Precompute adjacency. */
    static unsigned char M[16][16];
    static bool init = false;
    if (!init) {
        for (idx_t i = 0; i < 16; ++i) {
            unsigned a0 = i / 4, a1 = i % 4;
            for (idx_t j = 0; j < 16; ++j) {
                unsigned b0 = j / 4, b1 = j % 4;
                unsigned diff = (b0 > a1) ? b0 - a1 : a1 - b0;
                unsigned mx = (a0 > b1) ? a0 : b1;
                M[i][j] = (mx <= diff) ? 1 : 0;
            }
        }
        init = true;
    }

    /* dp[i] = number of sequences ending with column state i.
       Start with one column. */
    seq_t dp[16], next[16];
    for (idx_t i = 0; i < 16; ++i) dp[i] = (seq_t)1;

    /* Perform n transitions (n≥1). */
    for (idx_t step = 1; step <= n; ++step) {
        for (idx_t j = 0; j < 16; ++j) {
            seq_t s = 0;
            for (idx_t i = 0; i < 16; ++i) {
                if (M[i][j]) s += dp[i];
            }
            next[j] = s;
        }
        for (idx_t j = 0; j < 16; ++j) dp[j] = next[j];
    }

    /* Sum all sequences of length n+1. */
    seq_t total = 0;
    for (idx_t j = 0; j < 16; ++j) total += dp[j];
    return total;
}
