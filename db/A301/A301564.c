/*
 * A301564  Expansion of Product_{k>=0} (1 + x^(5*k+2))*(1 + x^(5*k+4)).
 * https://oeis.org/A301564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts congruent
 *            to 2 or 4 modulo 5
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *dp = NULL;
    static idx_t dp_max = (idx_t)-1;   /* maximum n for which dp has been computed */

    if (dp == NULL || n > dp_max) {
        /* allocate new array for the new maximum */
        seq_t *new_dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!new_dp) return 0;            /* unlikely, but just in case */

        /* initialize */
        for (idx_t i = 0; i <= n; ++i) new_dp[i] = 0;
        new_dp[0] = 1;

        /* process parts congruent to 2 modulo 5 */
        for (idx_t p = 2; p <= n; p += 5) {
            for (idx_t j = n; j >= p; --j) {
                new_dp[j] += new_dp[j - p];
            }
        }

        /* process parts congruent to 4 modulo 5 */
        for (idx_t p = 4; p <= n; p += 5) {
            for (idx_t j = n; j >= p; --j) {
                new_dp[j] += new_dp[j - p];
            }
        }

        /* replace old table */
        if (dp) free(dp);
        dp = new_dp;
        dp_max = n;
    }

    return dp[n];
}
