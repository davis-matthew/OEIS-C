/*
 * A269016  Number of 6Xn binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays with exactly one adjacent
 *            pair of ones (horizontally, diagonally, or antidiagonally)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Edge table: number of adjacent 1‑pairs formed between two consecutive
       columns with states `prev` and `cur`.          */
    static unsigned char edge[64][64];
    static int edge_inited = 0;
    if (!edge_inited) {
        for (int p = 0; p < 64; ++p) {
            for (int c = 0; c < 64; ++c) {
                unsigned char cnt = 0;
                /* horizontal adjacency */
                for (int r = 0; r < 6; ++r)
                    if (((p >> r) & 1) && ((c >> r) & 1))
                        ++cnt;
                /* diagonal (down‑right) adjacency */
                for (int r = 0; r < 5; ++r)
                    if (((p >> r) & 1) && ((c >> (r + 1)) & 1))
                        ++cnt;
                /* antidiagonal (down‑left) adjacency */
                for (int r = 1; r < 6; ++r)
                    if (((p >> r) & 1) && ((c >> (r - 1)) & 1))
                        ++cnt;
                edge[p][c] = cnt;
            }
        }
        edge_inited = 1;
    }

    /* dp_prev[s][k] : number of ways to fill columns up to current one
       with column pattern `s` and `k` adjacency pairs already counted
       (k ∈ {0,1}).  Two backing arrays suffice.               */
    seq_t dp_prev[64][2] = {{0}};
    seq_t dp_cur[64][2]  = {{0}};

    /* First column: no adjacency pairs can yet exist. */
    for (int s = 0; s < 64; ++s) {
        dp_prev[s][0] = 1;
        dp_prev[s][1] = 0;
    }

    /* Sweep over the remaining columns. */
    for (idx_t step = 1; step < n; ++step) {
        /* reset current DP */
        for (int s = 0; s < 64; ++s) {
            dp_cur[s][0] = 0;
            dp_cur[s][1] = 0;
        }

        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                unsigned char new_adj = edge[prev][cur];
                seq_t cnt0 = dp_prev[prev][0];
                seq_t cnt1 = dp_prev[prev][1];

                if (cnt0) {
                    int k = 0 + new_adj;
                    if (k <= 1)
                        dp_cur[cur][k] += cnt0;
                }
                if (cnt1) {
                    int k = 1 + new_adj;
                    if (k <= 1)
                        dp_cur[cur][k] += cnt1;
                }
            }
        }

        /* swap buffers */
        for (int s = 0; s < 64; ++s) {
            dp_prev[s][0] = dp_cur[s][0];
            dp_prev[s][1] = dp_cur[s][1];
        }
    }

    /* Sum over final states having exactly one adjacency pair. */
    seq_t answer = 0;
    for (int s = 0; s < 64; ++s)
        answer += dp_prev[s][1];

    return answer;
}
