/*
 * A283780  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×4 binary arrays with exactly two 1‑cells having more
 *            than two * 1‑neighbours (horizontal and diagonal). Computed by
 *            DP over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   precompute_viol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/precompute_viol.h"

seq_t a(idx_t n) {
    /* Precompute neighbour violation table once */
    static int done = 0;
    static uint8_t viol[16][16][16];
    if (!done) {
        precompute_viol(viol);
        done = 1;
    }

    /* DP state: dp[prevRow][currRow][violCount] */
    static seq_t dp[16][16][3];
    static seq_t newdp[16][16][3];
    /* zero initialize dp */
    for (size_t i = 0; i < 16; ++i)
        for (size_t j = 0; j < 16; ++j)
            for (size_t k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    /* initial row (row 1), prev row is all zeros */
    for (uint8_t row = 0; row < 16; ++row) {
        dp[0][row][0] = dp[0][row][0] + 1;
    }

    /* process rows 2 .. n */
    if (n >= 2) {
        for (idx_t r = 2; r <= n; ++r) {
            /* zero initialize newdp */
            for (size_t i = 0; i < 16; ++i)
                for (size_t j = 0; j < 16; ++j)
                    for (size_t k = 0; k < 3; ++k)
                        newdp[i][j][k] = 0;

            for (uint8_t prev = 0; prev < 16; ++prev) {
                for (uint8_t curr = 0; curr < 16; ++curr) {
                    for (uint8_t v = 0; v < 3; ++v) {
                        seq_t cnt = dp[prev][curr][v];
                        if (cnt == 0) continue;
                        for (uint8_t next = 0; next < 16; ++next) {
                            uint8_t add = viol[prev][curr][next];
                            uint8_t nv = v + add;
                            if (nv > 2) continue;
                            newdp[curr][next][nv] = newdp[curr][next][nv] + cnt;
                        }
                    }
                }
            }
            /* move newdp to dp */
            for (size_t i = 0; i < 16; ++i)
                for (size_t j = 0; j < 16; ++j)
                    for (size_t k = 0; k < 3; ++k)
                        dp[i][j][k] = newdp[i][j][k];
        }
    }

    /* after all rows processed, evaluate the last row (curr) with nextRow = 0 */
    seq_t answer = 0;
    for (uint8_t prev = 0; prev < 16; ++prev) {
        for (uint8_t curr = 0; curr < 16; ++curr) {
            for (uint8_t v = 0; v < 3; ++v) {
                seq_t cnt = dp[prev][curr][v];
                if (cnt == 0) continue;
                uint8_t add = viol[prev][curr][0];
                uint8_t nv = v + add;
                if (nv == 2) {
                    answer = answer + cnt;
                }
            }
        }
    }

    return answer;
}
