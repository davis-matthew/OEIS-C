/*
 * A281455  Expansion of Product_{k>=1} (1 + x^(7*k-2)).
 * https://oeis.org/A281455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct‑part partitions of n using parts 7k‑2,
 *            k≥1
 * Range:     u8 n <= 255, u16 n <= 641, u32 n <= 1849, u64 n <= 5912,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9832 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. the coefficient of x^n in ∏_{k≥1}(1+x^(7k‑2))) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = mem_new(seq_t, sz);
    if (!dp) return 0;          /* out of memory: unlikely in tests */

    /* Initialize to zero, and set dp[0] = 1 (empty subset) */
    for (idx_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;

    /* For each admissible part t = 7k‑2, update DP in reverse order */
    for (idx_t t = 5; t <= n; t += 7) {
        for (idx_t i = n; i >= t; --i) {
            dp[i] = dp[i] + dp[i - t];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
