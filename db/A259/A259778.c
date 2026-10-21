/*
 * A259778  Number of permutations p of [n] with no fixed points and
 *          displacement of elements restricted by five: 1 <= |p(i)-i| <= 5.
 * https://oeis.org/A259778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations p of [n] with no fixed points and
 *            |p(i)-i| between 1 and 5 for all i
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define K 5
#define B (2 * K + 1)
#define FULL ((1u << B) - 1)

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* State arrays: DP over 2^B = 2048 masks */
    static seq_t dp[1u << B];
    static seq_t dp_next[1u << B];

    /* Initialize DP */
    memset(dp, 0, sizeof(dp));

    /* Initial mask at i = 1 */
    uint32_t init_mask = 0;
    for (uint32_t p = 0; p < B; ++p) {
        /* Column index for position i = 1: col = 1 - K + p */
        idx_t col = (idx_t)(1 - K) + (idx_t)p;
        if (col >= 1 && col <= n) {
            if (col != 1) /* column 1 is the forbidden mid point */
                init_mask &= ~(1u << p); /* free column */
        } else {
            init_mask |= (1u << p); /* out of range -> occupied */
        }
    }
    dp[init_mask] = (seq_t)1;

    /* DP over rows i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        memset(dp_next, 0, sizeof(dp_next));
        for (uint32_t mask = 0; mask <= FULL; ++mask) {
            seq_t ways = dp[mask];
            if (!ways)
                continue;

            /* enumerate possible matches for row i */
            for (uint32_t p = 0; p < B; ++p) {
                if (p == K)          /* middle bit represents column i (fixed point) */
                    continue;
                if (mask & (1u << p))/* column already used */
                    continue;
                if ((mask & 1u) == 0 && p != 0) /* dropping an unused column */
                    continue;

                uint32_t newmask = mask | (1u << p);

                /* shift window: new mask for row i+1 */
                uint32_t shifted = newmask >> 1;
                if (i + K + 1 > n)   /* column i+K+1 is outside [1..n] */
                    shifted |= (1u << (B - 1));

                dp_next[shifted] += ways;
            }
        }
        /* swap dp arrays */
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* After processing all rows, all columns must be used -> mask == FULL */
    return dp[FULL];
}
