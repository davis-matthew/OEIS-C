/*
 * A326444  Sum of all the parts in the partitions of n into 8 squarefree
 *          parts.
 * https://oeis.org/A326444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (number of partitions of n into 8 square‑free parts)
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* test if x is square–free */
static inline bool is_squarefree(idx_t x)
{
    for (idx_t d = 2; d * d <= x; ++d)
        if (x % (d * d) == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* dp[k][m] is number of partitions of m into k square‑free parts */
    idx_t stride = n + 1;
    seq_t *dp = (seq_t *)calloc(9 * stride, sizeof(seq_t));
    if (!dp)
        return 0;           /* out of memory – should not happen in tests */

    dp[0] = 1;                /* one way to pick 0 parts that sum to 0 */

    /* build list of square‑free numbers up to n */
    idx_t *sqfree = (idx_t *)malloc(stride * sizeof(idx_t));
    if (!sqfree) {
        free(dp);
        return 0;
    }
    size_t sf_cnt = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (is_squarefree(i))
            sqfree[sf_cnt++] = i;

    /* DP – add each square‑free number once (unbounded usage) */
    for (size_t si = 0; si < sf_cnt; ++si) {
        idx_t s = sqfree[si];
        for (idx_t k = 1; k <= 8; ++k) {
            seq_t *dp_k   = dp + k * stride;
            seq_t *dp_km1 = dp + (k - 1) * stride;
            for (idx_t m = s; m <= n; ++m)
                dp_k[m] += dp_km1[m - s];
        }
    }

    seq_t count  = dp[8 * stride + n];
    seq_t result = (seq_t)n * count;

    free(dp);
    free(sqfree);
    return result;
}
