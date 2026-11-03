/*
 * A297515  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A297515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays where every 1 is *
 *            horizontally, diagonally or antidiagonally adjacent to exactly *
 *            1 or 3 neighboring 1s. (OEIS A297515) * * The algorithm uses
 *            dynamic programming over pairs of consecutive * rows. For a
 *            transition from two previous rows `pp` and `p` to a * new r...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 205 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if the last row `curr` is valid with the preceding row
 * `prev`.  Only left/right neighbours in `curr` and diagonal
 * neighbours in `prev` are available. */
static inline bool good_last(uint8_t prev, uint8_t curr)
{
    for (int c = 0; c < 4; ++c) {
        if (!(curr >> c & 1))
            continue;
        int cnt = 0;
        if (c > 0 && (curr >> (c - 1) & 1)) cnt++;
        if (c < 3 && (curr >> (c + 1) & 1)) cnt++;
        if (c > 0 && (prev >> (c - 1) & 1)) cnt++;
        if (c < 3 && (prev >> (c + 1) & 1)) cnt++;
        if (cnt != 1 && cnt != 3)
            return false;
    }
    return true;
}

/* Check whether a transition from rows `pp` -> `p` -> `n` is
 * allowed: each 1 in `p` must have 1 or 3 neighbours, counting
 * horizontal neighbours in `p`, diagonal neighbours in `pp`
 * and diagonal neighbours in `n`. */
static inline bool trans_ok(uint8_t pp, uint8_t p, uint8_t n)
{
    for (int c = 0; c < 4; ++c) {
        if (!(p >> c & 1))
            continue;
        int cnt = 0;
        /* horizontal neighbours in p */
        if (c > 0 && (p >> (c - 1) & 1)) cnt++;
        if (c < 3 && (p >> (c + 1) & 1)) cnt++;
        /* diagonal neighbours in pp (up-left/up-right) */
        if (c > 0 && (pp >> (c - 1) & 1)) cnt++;
        if (c < 3 && (pp >> (c + 1) & 1)) cnt++;
        /* diagonal neighbours in n (down-left/down-right) */
        if (c > 0 && (n >> (c - 1) & 1)) cnt++;
        if (c < 3 && (n >> (c + 1) & 1)) cnt++;
        if (cnt != 1 && cnt != 3)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the trivial first row case separately. */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t p = 0; p < 16; ++p) {
            if (good_last(0, p))
                ans += 1;
        }
        return ans;
    }

    /* DP tables: dp[prev][curr] counts arrangements up to the
     * current row, where prev is the pattern of the previous row
     * and curr is the pattern of the current row. */
    seq_t dp[16][16] = {0};
    seq_t ndp[16][16];
    /* Initial state: before the first row, pretend prevprev = 0. */
    for (uint8_t p = 0; p < 16; ++p)
        dp[0][p] = 1;

    /* Process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero new DP */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        /* Transitions */
        for (int pp = 0; pp < 16; ++pp) {
            for (int p = 0; p < 16; ++p) {
                seq_t val = dp[pp][p];
                if (!val) continue;
                for (int m = 0; m < 16; ++m) {
                    if (!trans_ok(pp, p, m))
                        continue;
                    ndp[p][m] += val;
                }
            }
        }
        /* Move to next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over final states where the last row is valid */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (good_last(prev, curr))
                ans += dp[prev][curr];
        }
    }
    return ans;
}
