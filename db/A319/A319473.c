/*
 * A319473  Number of partitions of n into exactly eight nonzero decimal
 *          palindromes.
 * https://oeis.org/A319473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into exactly eight * non‑zero
 *            decimal palindromes (order disregarded). The implementation *
 *            uses a standard coin‑change DP where the palindromes are the
 *            coins * and the number of parts is limited to 8. For each n the
 *            DP * computes the number of ways to reach n usi...
 * Range:     u8 n <= 27, u16 n <= 409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Pre‑compute all non‑zero decimal palindromes up to 10000
     * (the maximum n we need to handle).  The list is sorted. */
    static uint16_t pals[200];
    static size_t pals_cnt = 0;
    if (pals_cnt == 0) {
        size_t idx = 0;
        /* 1‑digit palindromes */
        for (int d = 1; d <= 9; ++d) pals[idx++] = d;
        /* 2‑digit palindromes */
        for (int d = 1; d <= 9; ++d) pals[idx++] = d * 10 + d;
        /* 3‑digit palindromes */
        for (int d = 1; d <= 9; ++d)
            for (int e = 0; e <= 9; ++e)
                pals[idx++] = d * 100 + e * 10 + d;
        /* 4‑digit palindromes */
        for (int d = 1; d <= 9; ++d)
            for (int e = 0; e <= 9; ++e)
                pals[idx++] = d * 1000 + e * 100 + e * 10 + d;
        pals_cnt = idx;
    }

    /* Allocate DP table: 9 rows (j = 0…8) × (n+1) columns. */
    size_t stride = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(stride * 9 * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failed */
    memset(dp, 0, stride * 9 * sizeof(seq_t));
    dp[0] = 1;                          /* dp[0][0] = 1 */

    /* Standard coin‑change DP: unlimited use of each palindrome,
     * counting exactly 8 parts. */
    for (size_t pi = 0; pi < pals_cnt && pals[pi] <= n; ++pi) {
        idx_t val = pals[pi];
        for (int j = 1; j <= 8; ++j) {
            size_t base_j   = (size_t)j * stride;
            size_t base_jm1 = (size_t)(j - 1) * stride;
            for (idx_t s = val; s <= n; ++s) {
                dp[base_j + s] += dp[base_jm1 + (s - val)];
            }
        }
    }

    seq_t result = dp[8 * stride + n];
    free(dp);
    return result;
}
