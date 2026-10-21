/*
 * A280128  Expansion of Product_{k>=2} (1 + mu(k)^2*x^k), where mu(k) is the
 *          Moebius function (A008683).
 * https://oeis.org/A280128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct squarefree parts
 *            > 1 (i.e., partitions counted by the generating function ∏_{k≥2}
 *            (1 + μ(k)² x^k).) This implementation uses dynamic programming
 *            with 0/1 knapsack over all squarefree integers ≤ n
 * Range:     u8 n <= 51, u16 n <= 133, u32 n <= 397, u64 n <= 1312,
 *            u128 n <= 4659, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Fast path for n = 0 */
    if (n == 0)
        return 1;

    /* Allocate DP array dp[0…n] of type seq_t */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return 0;   /* allocation failed; returned value is irrelevant */

    /* Initialize dp[0] = 1, others 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Count how many squarefree numbers exist up to n for allocation */
    size_t sf_cnt = 0;
    for (idx_t x = 2; x <= n; ++x)
        if (is_squarefree(x))
            ++sf_cnt;

    /* Allocate temporary array of squarefree integers */
    uint64_t *sf = (uint64_t *)malloc(sf_cnt * sizeof(uint64_t));
    if (!sf) {
        free(dp);
        return 0;
    }

    /* Fill the array */
    size_t pos = 0;
    for (idx_t x = 2; x <= n; ++x)
        if (is_squarefree(x))
            sf[pos++] = x;

    /* 0/1 knapsack DP over distinct squarefree parts */
    for (size_t i = 0; i < sf_cnt; ++i) {
        idx_t val = sf[i];
        for (idx_t j = n; j >= val; --j) {
            dp[j] += dp[j - val];
        }
    }

    seq_t result = dp[n];

    /* Clean up */
    free(sf);
    free(dp);
    return result;
}
