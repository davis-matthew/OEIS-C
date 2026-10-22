/*
 * A323799  Number of permutations p of [n] such that max_{j=1..n} |p(j)-j| =
 *          3.
 * https://oeis.org/A323799
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002526(n) - A002524(n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A002526(n): permutations of [n] with max |p(i)-i| <= 3   */
/*   A002524(n): permutations of [n] with max |p(i)-i| <= 2   */
/*   Count by transfer‑matrix DP over the band of width 2d+1.   */

#include <stdbool.h>

/* Maximum d needed (3).  Width = 2d+1 = 7  ->  128 states. */
#define MAX_D 3
#define MAX_WIDTH (2*MAX_D+1)
#define MAX_STATES (1 << MAX_WIDTH)

/* Count permutations of [n] with max displacement <= d. */
static seq_t count_le(idx_t d, idx_t n)
{
    if (n == 0) return (seq_t)1;   /* empty permutation */

    int width = (int)d * 2 + 1;
    int max_mask = 1 << width;

    static seq_t dp[MAX_STATES];
    static seq_t next[MAX_STATES];

    /* clear dp array */
    for (int m = 0; m < max_mask; ++m) dp[m] = (seq_t)0;

    /* initial mask: columns < 1 or > n are already used */
    uint64_t init_mask = 0;
    for (int k = 0; k < width; ++k) {
        int col = (int)1 - (int)d + k;    /* column index relative to row 1 */
        if (col < 1 || col > (int)n) init_mask |= 1U << k;
    }
    dp[init_mask] = (seq_t)1;

    /* process rows 1..n */
    for (idx_t row = 1; row <= n; ++row) {
        /* clear next */
        for (int m = 0; m < max_mask; ++m) next[m] = (seq_t)0;

        for (int mask = 0; mask < max_mask; ++mask) {
            seq_t ways = dp[mask];
            if (ways == (seq_t)0) continue;

            /* try all allowed columns j with |row-j| <= d and 1 <= j <= n */
            for (int offset = - (int)d; offset <= (int)d; ++offset) {
                int j = (int)row + offset;
                if (j < 1 || j > (int)n) continue;

                int bit = j - ((int)row - (int)d);   /* 0 .. width-1 */
                if (mask & (1U << bit)) continue;   /* already used */

                uint64_t new_mask = mask | (1U << bit);
                /* shift for next row */
                new_mask >>= 1;

                /* set bounds for next row (row+1) */
                if ((int)row - (int)d + 1 < 1) new_mask |= 1U;               /* left edge */
                if ((int)row + (int)d + 1 > (int)n) new_mask |= 1U << (width-1); /* right edge */

                next[new_mask] = next[new_mask] + ways;
            }
        }
        /* move next to dp for the following iteration */
        for (int m = 0; m < max_mask; ++m) dp[m] = next[m];
    }

    /* all bits 1 corresponds to all columns 1..n used */
    int full_mask = max_mask - 1;
    return dp[full_mask];
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* offset starts at 3 */

    /* permutations with max displacement <= 3 minus those with <= 2 */
    seq_t val3 = count_le(3, n);
    seq_t val2 = count_le(2, n);
    return val3 - val2;
}
