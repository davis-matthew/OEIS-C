/*
 * A283491  Number of nX5 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A283491: number of n×5 binary arrays in which every 1 has at most
   three equal neighbors among its horizontal, vertical and antidiagonal
   positions, except for exactly one element that may violate this rule. */

#include <stdint.h>

static uint8_t vbreak[32][32][32];

/* pre‑compute the number of violating cells in the middle row of a
   3‑row triple (prev2, prev1, cur). */
static void precompute_vbreak(void) {
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            for (int k = 0; k < 32; ++k) {
                int v = 0;
                for (int col = 0; col < 5; ++col) {
                    if ((j >> col) & 1) {
                        int up       = (i >> col) & 1;
                        int down     = (k >> col) & 1;
                        int left     = (col > 0) ? ((j >> (col - 1)) & 1) : 0;
                        int right    = (col < 4) ? ((j >> (col + 1)) & 1) : 0;
                        int anti_up  = (col < 4) ? ((i >> (col + 1)) & 1) : 0;
                        int anti_down= (col > 0) ? ((k >> (col - 1)) & 1) : 0;
                        int neigh = up + down + left + right + anti_up + anti_down;
                        if (neigh > 3) v++;
                    }
                }
                vbreak[i][j][k] = (uint8_t)v;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* value outside the offset is irrelevant */

    static int precomped = 0;
    if (!precomped) { precompute_vbreak(); precomped = 1; }

    /* dp[vi][p2][p1] – number of partial arrays with vi (0 or 1) violations
       so far, last two rows equal to p2 (row r‑2) and p1 (row r‑1). */
    seq_t dp[2][32][32]  = {0};
    seq_t next[2][32][32] = {0};

    dp[0][0][0] = (seq_t)1;   /* start with no rows */

    for (idx_t row = 0; row < n; ++row) {
        /* clear next */
        for (int vi = 0; vi < 2; ++vi)
            for (int p2 = 0; p2 < 32; ++p2)
                for (int p1 = 0; p1 < 32; ++p1)
                    next[vi][p2][p1] = (seq_t)0;

        for (int p2 = 0; p2 < 32; ++p2) {
            for (int p1 = 0; p1 < 32; ++p1) {
                seq_t c0 = dp[0][p2][p1];
                if (c0) {
                    for (int cur = 0; cur < 32; ++cur) {
                        uint8_t v = vbreak[p2][p1][cur];
                        if (v == 0)          next[0][p1][cur] += c0;
                        else if (v == 1)      next[1][p1][cur] += c0;
                    }
                }
                seq_t c1 = dp[1][p2][p1];
                if (c1) {
                    for (int cur = 0; cur < 32; ++cur) {
                        uint8_t v = vbreak[p2][p1][cur];
                        if (v == 0)          next[1][p1][cur] += c1;
                        /* v >= 1 would exceed the allowed single violation */
                    }
                }
            }
        }

        /* swap dp and next for next iteration */
        for (int vi = 0; vi < 2; ++vi)
            for (int p2 = 0; p2 < 32; ++p2)
                for (int p1 = 0; p1 < 32; ++p1)
                    dp[vi][p2][p1] = next[vi][p2][p1];
    }

    /* final evaluation of the last row with no row below (cur == 0) */
    seq_t ans = (seq_t)0;
    for (int p2 = 0; p2 < 32; ++p2) {
        for (int p1 = 0; p1 < 32; ++p1) {
            seq_t c0 = dp[0][p2][p1];
            if (c0) {
                uint8_t vfinal = vbreak[p2][p1][0];
                if (vfinal == 1) ans += c0;
            }
            seq_t c1 = dp[1][p2][p1];
            if (c1) {
                uint8_t vfinal = vbreak[p2][p1][0];
                if (vfinal == 0) ans += c1;
            }
        }
    }

    return ans;
}
