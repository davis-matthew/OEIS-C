/*
 * A282396  Number of n X 5 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors.
 * https://oeis.org/A282396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0–1 arrays with no 1 having more than *
 *            three of its king‑move neighbors also equal to 1. * (Dynamic
 *            programming over rows with state = last two rows) * * The
 *            implementation uses a 32×32 DP table (rows are represented by *
 *            5‑bit masks) and precomputes a validity table for tri...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* idx_t and seq_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* do nothing for n == 0 (undefined offset) */
    if (n == 0) return 0;

    /* ------------------------------------------------------------------
     *  Precompute tables on the first call.  The tables are static so
     *  that subsequent calls are fast and the function is
     *  re‑entrantly safe.
     * ------------------------------------------------------------------ */
    static bool initialized = false;

    /* validity[above][current][below] == 1 if for every 1 in 'current'
     * the number of neighboring 1s in the three rows (above, current,
     * below) does not exceed 3. */
    static unsigned char validity[32][32][32];

    /* For each pair (above, current) store the list of possible
     * subsequent rows 'below'.  next_mask[idx][k] gives the k‑th
     * allowed 'below' row for the pair with index idx = above*32+current.
     * next_len[idx] is the number of such rows. */
    static unsigned char next_mask[1024][32];
    static unsigned char next_len[1024];

    if (!initialized) {
        for (int above = 0; above < 32; ++above) {
            for (int cur = 0; cur < 32; ++cur) {
                for (int below = 0; below < 32; ++below) {
                    bool ok = true;
                    /* Check every set bit in 'cur'. */
                    for (int c = 0; c < 5; ++c) {
                        if (!(cur & (1 << c))) continue;
                        int cnt = 0;
                        /* Neighbors in the above row */
                        for (int d = -1; d <= 1; ++d) {
                            int col = c + d;
                            if (col < 0 || col > 4) continue;
                            if (above & (1 << col)) ++cnt;
                        }
                        /* Left and right neighbors in the same row */
                        if (c > 0 && (cur & (1 << (c - 1)))) ++cnt;
                        if (c < 4 && (cur & (1 << (c + 1)))) ++cnt;
                        /* Neighbors in the below row */
                        for (int d = -1; d <= 1; ++d) {
                            int col = c + d;
                            if (col < 0 || col > 4) continue;
                            if (below & (1 << col)) ++cnt;
                        }
                        if (cnt > 3) { ok = false; break; }
                    }
                    validity[above][cur][below] = ok;
                }
            }
        }

        /* Build the next‑row lists for fast transition. */
        for (int above = 0; above < 32; ++above) {
            for (int cur = 0; cur < 32; ++cur) {
                int idx = above * 32 + cur;
                int len = 0;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (validity[above][cur][nxt]) {
                        next_mask[idx][len++] = (unsigned char)nxt;
                    }
                }
                next_len[idx] = (unsigned char)len;
            }
        }

        initialized = true;
    }

    /* ------------------------------------------------------------------
     *  Dynamic programming.  dp[prev][cur] counts ways for the
     *  partial array ending with rows (prev, cur) where 'prev' has been
     *  fully validated but 'cur' is waiting for its below partner.
     * ------------------------------------------------------------------ */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];

    /* Clear dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* Special case: arrays of length 1. */
    if (n == 1) {
        seq_t res = 0;
        for (int a = 0; a < 32; ++a)
            if (validity[0][a][0]) res += 1;
        return res;
    }

    /* Initialize for the first two rows.  The first row is checked
     * against a virtual above row of all zeros. */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            if (validity[0][a][b])
                dp[a][b] = 1;

    /* Add the remaining rows one by one.  After adding 'nxt' we verify
     * that the middle row 'cur' is valid using its above row 'prev'
     * and the new below row 'nxt'. */
    for (idx_t row = 2; row < n; ++row) {
        /* Zero newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;
        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t count = dp[prev][cur];
                if (count == 0) continue;
                int idx_pair = prev * 32 + cur;
                for (int k = 0; k < next_len[idx_pair]; ++k) {
                    int nxt = next_mask[idx_pair][k];
                    newdp[cur][nxt] += count;
                }
            }
        }
        /* Swap dp and newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Finally validate the last row, which has no below partner. */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t count = dp[prev][cur];
            if (count == 0) continue;
            if (validity[prev][cur][0]) result += count;
        }
    }

    return result;
}
