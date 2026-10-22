/*
 * A341074  Number of partitions of n into 5 distinct squarefree parts.
 * https://oeis.org/A341074
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 5 distinct squarefree
 *            parts
 * Range:     u8 n <= 57, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Count partitions of n into 5 distinct squarefree parts */
seq_t a(idx_t n)
{
    /* For n below the offset, return 0 (behavior is irrelevant) */
    if (n < 17)
        return 0;

    /* 1. Compute all squarefree numbers <= n */
    size_t len = (size_t)(n + 1);
    bool *sqfree = (bool*)malloc(len);
    for (size_t i = 0; i < len; ++i)
        sqfree[i] = true;

    /* Sieve: mark numbers divisible by a square as non‑squarefree */
    for (idx_t p = 2; (idx_t)(p * p) <= n; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= n; j += sq)
            sqfree[j] = false;
    }

    /* Gather squarefree numbers into an array */
    size_t count = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (sqfree[i]) ++count;

    idx_t *sqnums = (idx_t*)malloc(count * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (sqfree[i]) sqnums[pos++] = i;

    free(sqfree);

    /* 2. Dynamic programming over subsets of squarefree numbers */
    size_t dpSize = (size_t)(n + 1) * 6; /* k = 0..5 */
    seq_t *dp = (seq_t*)calloc(dpSize, sizeof(seq_t));
    dp[0] = 1;  /* dp[sum=0][k=0] = 1 */

    for (size_t si = 0; si < count; ++si) {
        idx_t x = sqnums[si];
        /* Traverse sums descending to avoid reuse of the same number */
        for (long long s = (long long)n; s >= (long long)x; --s) {
            for (int k = 5; k >= 1; --k) {
                size_t idx_cur  = (size_t)s * 6 + k;
                size_t idx_prev = (size_t)(s - x) * 6 + (k - 1);
                dp[idx_cur] += dp[idx_prev];
            }
        }
    }

    seq_t result = dp[(size_t)n * 6 + 5];

    free(sqnums);
    free(dp);
    return result;
}
