/*
 * A288341  Expansion of 1 / ((1-x)^2*(1-x^2)*(1-x^3)*...*(1-x^6)).
 * https://oeis.org/A288341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 350, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288341: coefficients of 1/((1-x)^2*(1-x^2)*(1-x^3)*(1-x^4)*(1-x^5)*(1-x^6)) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values up to the largest n requested */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* number of elements allocated (dp_size = computed_max_n + 1) */

    /* if the cache is not large enough, grow it and recompute all values up to n */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp) return 0;   /* unlikely, but guard against OOM */

        for (idx_t i = 0; i < new_size; ++i) new_dp[i] = 0;
        new_dp[0] = 1;               /* coefficient of x^0 */

        /* two copies of part 1 -> (1-x)^(-2) */
        for (idx_t i = 1; i < new_size; ++i) new_dp[i] += new_dp[i - 1];
        for (idx_t i = 1; i < new_size; ++i) new_dp[i] += new_dp[i - 1];

        /* parts 2 to 6 : (1-x^k)^(-1) for k = 2..6 */
        for (idx_t k = 2; k <= 6; ++k) {
            for (idx_t i = k; i < new_size; ++i) new_dp[i] += new_dp[i - k];
        }

        /* replace old cache */
        if (dp) free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
