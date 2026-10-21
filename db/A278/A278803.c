/*
 * A278803  Number of n X 4 0..2 arrays with rows in nondecreasing
 *          lexicographic order and columns in nonincreasing lexicographic
 *          order.
 * https://oeis.org/A278803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n X 4 0..2 arrays with rows in nondecreasing
 *            lexicographic order and columns in nonincreasing lexicographic
 *            order. Computed with a dynamic programming over the 81 possible
 *            row vectors and the 3-bit mask indicating which column pairs
 *            have already been resolved as left>right. The algorith...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 28, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Representation of a row vector (base‑3 digits) */
    typedef struct {
        unsigned d[4];   /* digits 0..2 in order left to right */
        unsigned val;    /* integer 0..80 corresponding to the row */
    } Row;

    static Row rows[81];
    static int initialized = 0;
    if (!initialized) {
        for (unsigned v = 0; v < 81; ++v) {
            unsigned x = v;
            rows[v].d[3] = x % 3; x /= 3;
            rows[v].d[2] = x % 3; x /= 3;
            rows[v].d[1] = x % 3; x /= 3;
            rows[v].d[0] = x % 3;
            rows[v].val = v;
        }
        initialized = 1;
    }

    /* Precompute transitions: for every (prev, mask) list of (next, newmask) that
       satisfy the constraints. */
    static int trans_next[81][8][81];   /* at most 81 next values per state */
    static int trans_mask[81][8][81];
    static int trans_len[81][8];
    static int trans_prepared = 0;
    if (!trans_prepared) {
        for (int prev = 0; prev < 81; ++prev) {
            for (int mask = 0; mask < 8; ++mask) {
                int len = 0;
                for (int next = prev; next < 81; ++next) {
                    int newMask = mask;
                    int ok = 1;
                    for (int j = 0; j < 3; ++j) {
                        if (mask & (1 << j)) continue;           /* already resolved   */
                        unsigned a = rows[next].d[j];
                        unsigned b = rows[next].d[j + 1];
                        if (a < b) { ok = 0; break; }           /* would violate order */
                        else if (a > b) newMask |= 1 << j;       /* resolve hierarchy */
                    }
                    if (ok) {
                        trans_next[prev][mask][len] = next;
                        trans_mask[prev][mask][len] = newMask;
                        ++len;
                    }
                }
                trans_len[prev][mask] = len;
            }
        }
        trans_prepared = 1;
    }

    /* DP tables: dp[prev][mask] = count of ways to reach this state after current row */
    static seq_t dp[81][8];
    static seq_t ndp[81][8];

    /* Initialize for the first row */
    for (int i = 0; i < 81; ++i)
        for (int m = 0; m < 8; ++m)
            dp[i][m] = (seq_t)0;
    for (int v = 0; v < 81; ++v) {
        /* First row must be nonincreasing: digits[j] >= digits[j+1] */
        int mask = 0;
        int ok = 1;
        for (int j = 0; j < 3; ++j) {
            unsigned a = rows[v].d[j];
            unsigned b = rows[v].d[j + 1];
            if (a < b) { ok = 0; break; }
            if (a > b) mask |= 1 << j;
        }
        if (!ok) continue;
        dp[v][mask] = (seq_t)1;
    }

    /* Process remaining rows (if any) */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero ndp */
        for (int i = 0; i < 81; ++i)
            for (int m = 0; m < 8; ++m)
                ndp[i][m] = (seq_t)0;

        for (int prev = 0; prev < 81; ++prev) {
            for (int mask = 0; mask < 8; ++mask) {
                seq_t val = dp[prev][mask];
                if (!val) continue;
                int len = trans_len[prev][mask];
                for (int t = 0; t < len; ++t) {
                    int next = trans_next[prev][mask][t];
                    int nmask = trans_mask[prev][mask][t];
                    ndp[next][nmask] += val;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 81; ++i)
            for (int m = 0; m < 8; ++m)
                dp[i][m] = ndp[i][m];
    }

    /* Sum all states to obtain the final answer */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 81; ++i)
        for (int m = 0; m < 8; ++m)
            result += dp[i][m];
    return result;
}
