/*
 * A282395  Number of nX4 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors.
 * https://oeis.org/A282395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Constrained 0/1 matrices of size n×4.
   The function counts all n×4 binary arrays in which 
   every 1 has at most three 1‑neighbouring cells among its
   king-move neighbors (up, down, left, right, and the four diagonals).
   The computation uses dynamic programming over 4‑bit row masks.
 */

#include <stddef.h>   /* for size_t and NULL */
#include <string.h>   /* for memset, memcpy */

static unsigned char valid_mask[16][16][16];
static int valid_computed = 0;

static void compute_valid_masks(void)
{
    if (valid_computed) return;
    for (int p = 0; p < 16; ++p) {
        for (int c = 0; c < 16; ++c) {
            for (int n = 0; n < 16; ++n) {
                int ok = 1;
                for (int j = 0; j < 4; ++j) {
                    if (!((c >> j) & 1)) continue;
                    int cnt = 0;
                    /* above row */
                    if (j > 0 && ((p >> (j - 1)) & 1)) ++cnt;
                    if ((p >> j) & 1) ++cnt;
                    if (j < 3 && ((p >> (j + 1)) & 1)) ++cnt;
                    /* same row */
                    if (j > 0 && ((c >> (j - 1)) & 1)) ++cnt;
                    if (j < 3 && ((c >> (j + 1)) & 1)) ++cnt;
                    /* below row */
                    if (j > 0 && ((n >> (j - 1)) & 1)) ++cnt;
                    if ((n >> j) & 1) ++cnt;
                    if (j < 3 && ((n >> (j + 1)) & 1)) ++cnt;
                    if (cnt > 3) { ok = 0; break; }
                }
                valid_mask[p][c][n] = (unsigned char)ok;
            }
        }
    }
    valid_computed = 1;
}

seq_t a(idx_t n)
{
    compute_valid_masks();

    /* dp[prev][cur] – number of ways to build rows processed so far
       ending with two rows of masks prev (i‑1) and cur (i).                 */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];
    memset(dp, 0, sizeof dp);

    /* first row: prev = 0 (virtual empty row above), cur = any mask */
    for (int r = 0; r < 16; ++r) dp[0][r] = 1;

    /* process rows 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        memset(ndp, 0, sizeof ndp);
        for (int prev = 0; prev < 16; ++prev) {
            for (int cur = 0; cur < 16; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (valid_mask[prev][cur][nxt]) {
                        ndp[cur][nxt] += ways;
                    }
                }
            }
        }
        memcpy(dp, ndp, sizeof dp);
    }

    /* final row: below row is all zeros */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            if (valid_mask[prev][cur][0]) {
                result += dp[prev][cur];
            }
        }
    }
    return result;
}
