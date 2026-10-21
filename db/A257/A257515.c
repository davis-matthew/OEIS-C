/*
 * A257515  Number of 3-generalized 2-Motzkin paths of length n with no level
 *          steps H=(3,0) at odd level.
 * https://oeis.org/A257515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3‑generalized 2‑Motzkin paths of length n *
 *            with no level steps H=(3,0) at odd level. * * Computation:
 *            dynamic programming on horizontal position and height. * States:
 *            dp[x][h] = number of valid paths from (0,0) to (x,h), * where
 *            the y‑coordinate is always non‑negative and horizo...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 63, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case: the empty path. */
    if (n == 0) return 1;

    /* Number of columns needed for each row: 0 … n. */
    size_t len = (size_t)n + 1;

    /* Allocate four rows of long enough size and zero them. */
    seq_t *dp = (seq_t *)calloc(4 * len, sizeof(seq_t));
    if (!dp) return 0;                /* allocation failed – return 0 */

    /* Initialize: dp[0][0] = 1. */
    dp[0 * len + 0] = 1;

    /* Main DP: iterate over horizontal positions. */
    for (idx_t x = 1; x <= n; ++x) {
        size_t cur = (size_t)(x % 4);
        size_t p1  = (size_t)((x + 3) % 4);   /* dp[x‑1] index */
        size_t p3  = (size_t)((x + 1) % 4);   /* dp[x‑3] index, valid when x≥3 */

        /* Compute dp[x][h] for all possible heights. */
        for (idx_t h = 0; h <= x; ++h) {
            seq_t val = 0;

            /* Up step: from height h‑1. */
            if (h > 0) {
                val += dp[p1 * len + (h - 1)];
            }

            /* Down step: from height h+1. */
            if (h + 1 <= x - 1) {
                val += dp[p1 * len + (h + 1)];
            }

            /* Horizontal step: two types, only when height is even. */
            if (x >= 3 && (h & 1) == 0) {
                seq_t tmp = dp[p3 * len + h];
                val += tmp + tmp;          /* multiply by 2 without overflow risk */
            }

            dp[cur * len + h] = val;
        }
        /* No need to clear the rest of the row; it will be overwritten next time. */
    }

    /* Result is the number of paths ending at height 0 after n steps. */
    seq_t result = dp[(size_t)(n % 4) * len + 0];
    free(dp);
    return result;
}
