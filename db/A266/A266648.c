/*
 * A266648  Expansion of Product_{k>=1} (1 + x^(3*k)) / (1 - x^k).
 * https://oeis.org/A266648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in ∏_{k≥1} (1 + x^{3k}) / (1 –
 *            x^k)
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 110, u64 n <= 358,
 *            u128 n <= 1252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate the dynamic-programming array */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp) return 0;          /* unlikely in normal tests */
    dp[0] = 1;                  /* start with the empty partition */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* unlimited copies of part k: factor 1/(1 - x^k) */
        for (size_t j = (size_t)k; j <= N; ++j)
            dp[j] += dp[j - (size_t)k];

        /* optional overlined part of size 3k: factor (1 + x^{3k}) */
        size_t shift = 3 * (size_t)k;
        if (shift <= N) {
            for (size_t j = N; j >= shift; --j)
                dp[j] += dp[j - shift];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
