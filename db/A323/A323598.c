/*
 * A323598  "Word binomial coefficient" for (x|y) where y is the first n
 *          symbols of the Thue-Morse sequence (A010060) and x is the first 2n
 *          symbols.
 * https://oeis.org/A323598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of ways the first n symbols of the Thue–Morse
 *            * sequence can appear as a (scattered) subsequence of its
 *            initial 2n symbols. * Computation is performed by dynamic
 *            programming counting subsequences
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Allocate DP array: dp[i] = number of ways y[0..i-1] appears in processed part of x */
    seq_t *dp = mem_new(seq_t, (n + 1));
    if (!dp) abort();
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* empty pattern matches in exactly one way */

    /* Pre‑compute the first n bits of the Thue–Morse sequence (y) */
    bool *y = (bool *)malloc(n * sizeof(bool));
    if (!y) { mem_free(dp); abort(); }
    for (idx_t i = 0; i < n; ++i)
        y[i] = __builtin_parity((unsigned long long)i);

    /* Process the first 2n symbols of the Thue–Morse sequence (x) */
    for (idx_t j = 0; j < 2 * n; ++j) {
        bool bx = __builtin_parity((unsigned long long)j);
        /* Update DP from the back to avoid overwriting needed values */
        for (idx_t i = n; i >= 1; --i) {
            if (y[i - 1] == bx)
                dp[i] = dp[i] + dp[i - 1];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    free(y);
    return result;
}
