/*
 * A292414  a(n) = [x^n] Product_{k>=1} (1 + 2^n*x^k).
 * https://oeis.org/A292414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = [x^n] ∏_{k≥1} (1 + 2^n·x^k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 168 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the aforementioned coefficient using a standard DP
   over the distinct-sum partition generating function. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return 1;

    /* Allocate DP array of size n+1 */
    idx_t size = n + 1;
    seq_t *dp = mem_new(seq_t, size);
    if (!dp) return 0;                  /* out of memory → return 0 */

    /* Initialize dp[0] = 1, others = 0 */
    for (idx_t i = 0; i < size; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Constant factor 2^n used for every part */
    seq_t q = pow2(n);

    /* Classic subset‐sum DP: each part k may be used at most once */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t j = n; j >= k; --j) {
            dp[j] = dp[j] + dp[j - k] * q;
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
