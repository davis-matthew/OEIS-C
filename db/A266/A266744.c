/*
 * A266744  G.f.: 1/((1-t^4)*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)).
 * https://oeis.org/A266744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to x1 + 3x3 + 4x4
 *            + 5x5 + 7x7 = n (i.e. partitions of n using parts 1,3,4,5,7)
 * Range:     u8 n <= 30, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t parts[5] = {1, 3, 4, 5, 7};
    size_t sz = (size_t)n + 1;

    seq_t *dp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;                      /* allocation failure */

    dp[0] = 1;
    for (size_t i = 1; i < sz; ++i) dp[i] = 0;

    for (int p = 0; p < 5; ++p) {
        idx_t k   = parts[p];
        size_t sk = (size_t)k;
        if (sk > n) continue;              /* part larger than n */
        for (size_t i = sk; i < sz; ++i) {
            dp[i] += dp[i - sk];
        }
    }

    seq_t result = dp[sz - 1];
    free(dp);

    return result;
}
