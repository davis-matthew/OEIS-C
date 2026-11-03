/*
 * A332014  Number of compositions (ordered partitions) of n into distinct
 *          hexagonal numbers.
 * https://oeis.org/A332014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S subset of distinct hexagonal numbers summing to n}
 *            of |S|!
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Generate all hexagonal numbers ≤ n.  */
    idx_t hex[200];           /* more than enough for n ≤ 10000 (≈70 hexagonals) */
    size_t m = 0;
    for (idx_t k = 1;; ++k) {
        idx_t h = k * (2 * k - 1);
        if (h > n) break;
        hex[m++] = h;
    }

    /* DP table: dp[k*stride + s] = number of subsets of size k summing to s. */
    size_t stride = (size_t)n + 1;                      /* columns = 0 … n */
    seq_t *dp = mem_new(seq_t, (m + 1) * stride);
    if (!dp) return 0;  /* allocation failure – unlikely in tests */

    dp[0] = 1;           /* empty subset */

    /* Subset DP with 0/1 inclusion. */
    for (size_t idx = 0; idx < m; ++idx) {
        idx_t h = hex[idx];
        for (size_t k = m; k > 0; --k) {            /* k from m down to 1 */
            for (size_t s = n; s >= (size_t)h; --s) {
                size_t from = (k - 1) * stride + (s - (size_t)h);
                size_t to   = k * stride + s;
                dp[to] += dp[from];
            }
        }
    }

    /* factorials up to m. */
    seq_t *fact = mem_new(seq_t, m + 1);
    if (!fact) {
        mem_free(dp);
        return 0;
    }
    fact[0] = 1;
    for (size_t i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Sum over all subset sizes. */
    seq_t res = 0;
    for (size_t k = 0; k <= m; ++k) {
        seq_t ways = dp[k * stride + n];
        res += ways * fact[k];
    }

    mem_free(dp);
    mem_free(fact);
    return res;
}
