/*
 * A283945  Number of nX3 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A283945: Number of n×3 binary arrays with exactly one 1 that may
 * have more than three of its horizontal, diagonal and antidiagonal
 * neighbors (vertical neighbors are NOT counted).
 *
 * The function uses a dynamic‑programming approach on row triples.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n = 1 has no array with exactly one violating element */
    if (n == 1)
        return (seq_t)0;

    /* Pre‑compute violation counts for all triples (top, middle, bottom)
       where neighbors are the six cells:
       left, right, top‑left, top‑right, bottom‑left, bottom‑right */
    static unsigned char viol[8][8][8];
    static bool viol_inited = false;

    if (!viol_inited) {
        for (int t = 0; t < 8; ++t) {
            for (int m = 0; m < 8; ++m) {
                for (int b = 0; b < 8; ++b) {
                    unsigned char v = 0;
                    for (int j = 0; j < 3; ++j) {
                        if (!(m & (1U << j))) continue;   /* cell is 0 */

                        int cnt = 0;
                        /* horizontal (middle row) */
                        if (j > 0 && (m & (1U << (j - 1)))) ++cnt;
                        if (j < 2 && (m & (1U << (j + 1)))) ++cnt;
                        /* diagonal neighbors (top‑left/top‑right) */
                        if (j > 0 && (t & (1U << (j - 1)))) ++cnt;
                        if (j < 2 && (t & (1U << (j + 1)))) ++cnt;
                        /* diagonal neighbors (bottom‑left/bottom‑right) */
                        if (j > 0 && (b & (1U << (j - 1)))) ++cnt;
                        if (j < 2 && (b & (1U << (j + 1)))) ++cnt;
                        /* vertical neighbors are NOT counted */

                        if (cnt > 3) ++v;   /* this 1 violates */
                    }
                    viol[t][m][b] = v;
                }
            }
        }
        viol_inited = true;
    }

    /* DP: dp[prev][curr][k]  - number of ways to reach current state
       with prev row = prev, current row = curr, and k violations already seen (k = 0 or 1). */
    static seq_t dp[8][8][2];
    static seq_t dp_next[8][8][2];

    /* reset tables */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 2; ++k) {
                dp[i][j][k] = (seq_t)0;
                dp_next[i][j][k] = (seq_t)0;
            }

    /* initialise using the first two real rows; top ghost row = 0 */
    for (int r0 = 0; r0 < 8; ++r0) {
        for (int r1 = 0; r1 < 8; ++r1) {
            unsigned char v0 = viol[0][r0][r1];
            if (v0 > 1) continue;
            dp[r0][r1][v0] += (seq_t)1;
        }
    }

    /* process remaining rows (row index 2 … n-1) */
    for (idx_t row = 2; row < n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 2; ++k)
                    dp_next[i][j][k] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                for (int k = 0; k < 2; ++k) {
                    seq_t val = dp[prev][curr][k];
                    if (val == (seq_t)0) continue;
                    for (int nxt = 0; nxt < 8; ++nxt) {
                        unsigned char vmid = viol[prev][curr][nxt];
                        if (k + vmid > 1) continue;
                        dp_next[curr][nxt][k + vmid] += val;
                    }
                }
            }
        }

        /* copy dp_next back to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 2; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* final evaluation: count violations for the last real row against bottom
       ghost row (0). Exactly one violation in the whole array is required. */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            for (int k = 0; k < 2; ++k) {
                seq_t val = dp[prev][curr][k];
                if (val == (seq_t)0) continue;
                unsigned char vlast = viol[prev][curr][0];
                if (k + vlast == 1) ans += val;
            }
        }
    }

    return ans;
}
