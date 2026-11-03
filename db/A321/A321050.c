/*
 * A321050  Number of permutations of [n] with no fixed points where the
 *          maximal displacement of an element equals four.
 * https://oeis.org/A321050
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A259777(n) - A079997(n) Count derangements of [n] with
 *            maximal displacement exactly 4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 976 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t count_derangements_match(idx_t n, int b)
{
    /* Number of bits in the sliding window (columns i-b … i+b). */
    const int W = 2 * b + 1;
    /* Mask size (2^W).  W ≤ 9 for b ≤ 4. */
    const int SZ = 1 << W;

    /* Permanent arrays – large enough for the maximal width (9). */
    static seq_t dp[512];
    static seq_t ndp[512];

    /* Initial mask: bits corresponding to columns outside [1,n] are set. */
    int init_mask = 0;
    for (int idx = 0; idx < W; ++idx) {
        int64_t col = (int64_t)1 - b + idx;          /* column index for bit idx */
        if (col < 1 || col > (int64_t)n)
            init_mask |= 1 << idx;
    }

    /* Initialize dp */
    for (int s = 0; s < SZ; ++s) dp[s] = 0;
    dp[init_mask] = 1;

    /* Process rows i = 1 … n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Clear ndp */
        for (int s = 0; s < SZ; ++s) ndp[s] = 0;

        /* Allowed j values for this row */
        idx_t lo = (i > (idx_t)b) ? i - b : 1;
        idx_t hi = (i + (idx_t)b > n) ? n : i + (idx_t)b;

        for (int s = 0; s < SZ; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;

            for (idx_t j = lo; j <= hi; ++j) {
                if (j == i) continue;          /* no fixed point */

                /* Position of column j in the current window. */
                int bitpos = (int)(j - (i - (idx_t)b));
                if (bitpos < 0 || bitpos >= W) continue;

                if ((s >> bitpos) & 1) continue;  /* column already used */

                int new_mask = s | (1 << bitpos);

                /* Ensure that the leftmost column (i-b) is matched now. */
                if ((new_mask & 1) == 0) continue;

                int next_mask = new_mask >> 1;

                /* If the new rightmost column lies outside [1,n], it is
                   considered already used. */
                if (i + (idx_t)b + 1 > n)
                    next_mask |= 1 << (W - 1);

                ndp[next_mask] += cur;
            }
        }

        /* Move to next row */
        for (int s = 0; s < SZ; ++s) dp[s] = ndp[s];
    }

    /* Sum over all final states */
    seq_t total = 0;
    for (int s = 0; s < SZ; ++s) total += dp[s];
    return total;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                 /* offset at 4 */

    seq_t tot4 = count_derangements_match(n, 4);
    seq_t tot3 = count_derangements_match(n, 3);

    return tot4 - tot3;
}
