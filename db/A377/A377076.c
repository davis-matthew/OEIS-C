/*
 * A377076  G.f.: Sum_{k>=0} x^(6*k^2) / Product_{j=1..6*k-1} (1 - x^j).
 * https://oeis.org/A377076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k\ge 1,\, 6k^2 \le n} p_{6k-1}(n-6k^2) where p_m(t)
 *            is the number of integer partitions of t using parts not
 *            exceeding m. This follows from the generating function
 *            \displaystyle\sum_{k\ge 0}
 *            \frac{x^{6k^2}}{\prod_{j=1}^{6k-1}(1-x^j)}
 * Range:     u8 n <= 27, u16 n <= 71, u32 n <= 199, u64 n <= 634,
 *            u128 n <= 2209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The first non‑zero term occurs at n = 6.  For n < 6 the sum is
       empty, so a(n) = 0. */
    if (n < 6)
        return (seq_t)0;

    /* Compute the largest k such that 6*k^2 <= n. */
    idx_t max_k = 0;
    while (1) {
        idx_t t = 6 * max_k * max_k;
        if (t > n)
            break;
        ++max_k;
    }
    --max_k;               /* max_k is now the largest admissible value */

    /* If no admissible k exists (this can happen only for n < 6, already
       handled above), return 0. */
    if (max_k == 0)
        return (seq_t)0;

    /* Maximal part size needed in the dynamic program. */
    idx_t max_m = 6 * max_k - 1;

    /* Allocate an array dp[0..n] that will hold the number of partitions
       of each size using parts up to the current m. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* Allocation failed; return 0 as a safe fallback. */

    /* Initialise dp[0] = 1 (empty partition), all others 0. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    seq_t result = (seq_t)0;

    /* Iterate over the part sizes, updating dp incrementally.  When we
       reach the part size that corresponds to the current k, we add the
       number of partitions of the required remainder to the answer. */
    idx_t k = 1;                                     /* current k   */
    idx_t target_m = 6 * k - 1;                       /* part size for this k */
    idx_t remainder = n - 6 * k * k;                  /* n - 6k^2     */

    for (idx_t m = 1; m <= max_m; ++m) {
        /* Incorporate part size m into the partition count. */
        for (idx_t i = m; i <= n; ++i)
            dp[i] += dp[i - m];

        /* When we reach the exact m needed for the current k, add the
           count for the remainder to the result. */
        if (m == target_m) {
            result += dp[remainder];

            /* Advance to the next admissible k. */
            ++k;
            if (k <= max_k) {
                target_m = 6 * k - 1;
                remainder = n - 6 * k * k;
            }
        }
    }

    free(dp);
    return result;
}
