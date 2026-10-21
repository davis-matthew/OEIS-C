/*
 * A288166  Expansion of x^5/((1-x^5)*(1-x^4)*(1-x^8)*(1-x^12)*(1-x^16)).
 * https://oeis.org/A288166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_5(n/4) if n ≡ 0 (mod 4) * a(n) = p_5((n+15)/4) if n ≡ 1
 *            (mod 4) * a(n) = p_5((n+10)/4) if n ≡ 2 (mod 4) * a(n) =
 *            p_5((n+5)/4) if n ≡ 3 (mod 4) * where p_5(k) is the number of
 *            partitions of k into exactly 5 parts. * * p_k(n) is computed by
 *            the recurrence * p_k(n) = p_k(n-k) + p_{k-1}(n-1)...
 * Range:     u8 n <= 96, u16 n <= 444, u32 n <= 7476, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Compute the argument m of p_5 according to n mod 4 */
    idx_t remainder = n & 3U;
    unsigned __int128 tmp = n;
    idx_t m;
    if (remainder == 0)          m = (idx_t)(tmp / 4);
    else if (remainder == 1)     m = (idx_t)((tmp + 15) / 4);
    else if (remainder == 2)     m = (idx_t)((tmp + 10) / 4);
    else                         m = (idx_t)((tmp + 5)  / 4);

    /* Ensure we have a table of p_k values up to k=5 and n=m */
    size_t needed = (size_t)m + 1;          /* indices 0..m inclusive */
    static seq_t **dp = NULL;               /* dp[k][n] */
    static size_t cacheSize = 0;            /* current table width */

    if (needed > cacheSize) {
        /* Free old table */
        if (dp) {
            for (int k = 0; k < 6; ++k) free(dp[k]);
            free(dp);
        }
        /* Allocate new table */
        dp = (seq_t **)malloc(6 * sizeof(seq_t *));
        for (int k = 0; k < 6; ++k) {
            dp[k] = (seq_t *)calloc(needed, sizeof(seq_t));
        }
        /* Initialize dp[0] */
        dp[0][0] = (seq_t)1;
        /* Compute the table for k = 1..5 */
        for (int k = 1; k < 6; ++k) {
            for (size_t s = 0; s < needed; ++s) {
                if (s < (size_t)k) {
                    dp[k][s] = (seq_t)0;
                } else {
                    seq_t part1 = dp[k][s - k];
                    seq_t part2 = dp[k - 1][s - 1];
                    dp[k][s] = part1 + part2;
                }
            }
        }
        cacheSize = needed;
    }

    return dp[5][m];
}
