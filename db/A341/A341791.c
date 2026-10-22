/*
 * A341791  Number of partitions of n into 8 nonzero tetrahedral numbers.
 * https://oeis.org/A341791
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 8 non‑zero tetrahedral
 *            numbers. Computed by dynamic programming
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* required for memset in C (not used in C++) */

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;

    /* Build list of tetrahedral numbers ≤ n */
    idx_t k = 1;
    idx_t tcnt = 0;
    seq_t tet[256];            /* enough for n ≤ 10000 (k ≤ 50) */
    while (1) {
        seq_t T = (seq_t)k * (k + 1) * (k + 2) / 6;
        if (T > n)
            break;
        tet[tcnt++] = T;
        ++k;
    }

    /* DP table: rows 0..n, cols 0..8 (inclusive) */
    size_t rows = (size_t)n + 1;
    const size_t cols = 9;
    size_t total = rows * cols;

#ifdef __cplusplus
    seq_t *dp = new seq_t[total];        /* default‑constructed (zero) */
#else
    seq_t *dp = calloc(total, sizeof(seq_t));   /* zeroed */
#endif
    if (!dp)
        return 0;  /* allocation failure */

    dp[0] = 1;  /* dp[0][0] = 1 */

    for (idx_t ti = 0; ti < tcnt; ++ti) {
        idx_t val = (idx_t)tet[ti];
        for (idx_t s = val; s <= n; ++s) {
            seq_t *dest = dp + (size_t)s * cols;
            seq_t *src  = dp + (size_t)(s - val) * cols;
            for (size_t p = 1; p <= 8; ++p) {
                dest[p] += src[p - 1];
            }
        }
    }

    seq_t result = dp[(size_t)n * cols + 8];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
