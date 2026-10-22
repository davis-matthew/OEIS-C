/*
 * A364135  Let d_r d_{r-1} ... d_1 d_0 be the decimal expansion of n; a(n) is
 *          the number of nonnegative integer solutions x_r ... x_0 to the
 *          Diophantine equation d_r*x_r + ... + d_0*x_0 = n.
 * https://oeis.org/A364135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = number of non‑negative integer solutions to d_r·x_r + … +
 *            d_0·x_0 = n, where d_i are the decimal digits of n (zeros are
 *            ignored). The function computes this by dynamic programming over
 *            the digits
 * Range:     u8 n <= 110, u16 n <= 636, u32 n <= 636, u64 n <= 636,
 *            u128 n <= 636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case for n = 0 is not required (offset 1). */
    /* Extract non‑zero decimal digits of n. */
    uint8_t digits[64]; /* enough for any 64‑bit integer */
    size_t md = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[md++] = 0; /* not used, just to avoid empty loop */
    } else {
        while (tmp > 0) {
            uint8_t d = (uint8_t)(tmp % 10);
            if (d != 0) digits[md++] = d;
            tmp /= 10;
        }
    }
    if (md == 0) /* all digits are zero -> only n = 0, handled earlier */
        return (seq_t)1;

    /* Allocate DP array of size n+1, initialized to 0. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0; /* failure fallback */

    dp[0] = (seq_t)1; /* base case: one way to make sum 0 */

    /* Process each non‑zero digit as an unlimited coin of value d. */
    for (size_t dpos = 0; dpos < md; ++dpos) {
        idx_t d = (idx_t)digits[dpos];
        /* In‑place update: dp[s] += dp[s-d] for s >= d. */
        for (idx_t s = d; s <= n; ++s) {
            dp[s] += dp[s - d];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
