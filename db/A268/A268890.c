/*
 * A268890  Number of 5Xn binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count binary 5×n arrays with exactly one horizontally or
 *            antidiagonally adjacent pair of 1s. Implementation uses dynamic
 *            programming over the 32 possible column states
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* a(1) = 0, and for n<=0 the sequence is undefined */

    /* Precompute number of adjacencies between any two 5‑bit column states. */
    static uint8_t edgeCount[32][32];
    static int initialized = 0;
    if (!initialized) {
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                unsigned int k = 0;
                /* Horizontal adjacency (same row) */
                for (int r = 0; r < 5; ++r)
                    if (((a >> r) & 1U) && ((b >> r) & 1U))
                        ++k;
                /* Antidiagonal adjacency: left row r connects to right row r-1 (r>=1) */
                for (int r = 1; r < 5; ++r)
                    if (((a >> r) & 1U) && ((b >> (r - 1)) & 1U))
                        ++k;
                edgeCount[a][b] = (uint8_t)k;
            }
        }
        initialized = 1;
    }

    /* DP arrays: cur[state][edgesSeen] and next[state][edgesSeen], edgesSeen = 0 or 1 */
    seq_t cur[32][2] = {0}, next[32][2];
    for (int s = 0; s < 32; ++s) cur[s][0] = 1;  /* first column, no edges so far */

    for (idx_t col = 1; col < n; ++col) {
        /* zero next array */
        for (int s = 0; s < 32; ++s) next[s][0] = next[s][1] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            seq_t v0 = cur[prev][0];
            seq_t v1 = cur[prev][1];
            if (v0 == 0 && v1 == 0) continue;

            for (int nextState = 0; nextState < 32; ++nextState) {
                uint8_t k = edgeCount[prev][nextState];

                /* Transition from state cur[prev][0] */
                if ((v0 != 0) && k <= 1) {
                    next[nextState][k] += v0;
                }
                /* Transition from state cur[prev][1] */
                if ((v1 != 0) && (k + 1 <= 1)) {
                    next[nextState][1] += v1;  /* k is 0 here since total must stay <=1 */
                }
            }
        }
        /* swap cur and next */
        for (int s = 0; s < 32; ++s) {
            cur[s][0] = next[s][0];
            cur[s][1] = next[s][1];
        }
    }

    /* Sum all states with exactly one adjacency encountered */
    seq_t result = 0;
    for (int s = 0; s < 32; ++s)
        result += cur[s][1];

    return result;
}
