/*
 * A282397  Number of nX6 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors.
 * https://oeis.org/A282397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays in which no 1 has more than *
 *            three king‑move neighbors that are 1. The implementation uses a
 *            * row‑by‑row dynamic programming with a 3‑row sliding window
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool initialized = false;
static uint64_t trans_mask[64][64];

/* check whether the middle row satisfies the constraint given the
 * above and below rows (rows are 6-bit patterns). */
static bool check_row(uint64_t above, uint64_t middle, uint64_t below)
{
    for (int col = 0; col < 6; ++col) {
        if ((middle >> col) & 1ull) {        /* this cell is 1 */
            int cnt = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                uint64_t row;
                if (dr == -1) row = above;
                else if (dr == 0) row = middle;
                else row = below;
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int c2 = col + dc;
                    if (c2 < 0 || c2 >= 6) continue;
                    if ((row >> c2) & 1ull) ++cnt;
                }
            }
            if (cnt > 3) return false;
        }
    }
    return true;
}

/* precompute all admissible transitions */
static void init_transitions(void)
{
    if (initialized) return;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            uint64_t mask = 0;
            for (int c = 0; c < 64; ++c) {
                if (check_row((uint64_t)a, (uint64_t)b, (uint64_t)c))
                    mask |= 1ULL << c;
            }
            trans_mask[a][b] = mask;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 explicitly. */
    if (n == 1) return (seq_t)64;

    init_transitions();

    static seq_t dp_curr[64][64];
    static seq_t dp_next[64][64];

    /* Initialize: first two rows.  The upper row for the first row is
     * treated as 0 (all zeros). */
    memset(dp_curr, 0, sizeof(dp_curr));
    for (int a = 0; a < 64; ++a) {
        uint64_t mask = trans_mask[0][a];          /* below row */
        while (mask) {
            int b = __builtin_ctzll(mask);
            dp_curr[a][b] += (seq_t)1;
            mask &= mask - 1;
        }
    }

    /* Process remaining rows: for row index i from 3 to n. */
    for (idx_t row = 3; row <= n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t cnt = dp_curr[a][b];
                if (!cnt) continue;
                uint64_t bits = trans_mask[a][b];
                while (bits) {
                    int c = __builtin_ctzll(bits);
                    dp_next[b][c] += cnt;
                    bits &= bits - 1;
                }
            }
        }
        /* swap dp_curr and dp_next for next iteration */
        memcpy(dp_curr, dp_next, sizeof(dp_curr));
    }

    /* Sum over states where the last row satisfies the constraint
     * with above row and with below row = 0. */
    seq_t ans = (seq_t)0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            if ((trans_mask[a][b] >> 0) & 1ULL) {
                ans += dp_curr[a][b];
            }
        }
    }
    return ans;
}
