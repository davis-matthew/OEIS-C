/*
 * A338678  Expansion of Product_{k>=1} 1 / (1 - 9^(k-1)*x^k).
 * https://oeis.org/A338678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1-9^(k-1)·x^k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Allocate a DP array of size n+1 (dp[0]..dp[n]) */
    size_t len = (size_t)(n + 1);
    seq_t *dp = mem_new(seq_t, len);
    if (!dp) return 0;                /* memory failure – return 0 */

    /* initialise all entries to 0 */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;                         /* base: one empty partition */

    seq_t w = 1;                       /* 9^(k-1) for current k */
    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1) w = mul_small(w, 9);      /* update w to 9^(k-1) */

        for (idx_t i = k; i <= n; ++i) {
            dp[i] += dp[i - k] * w;          /* add weight of a part of size k */
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
