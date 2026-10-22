/*
 * A313033  Coordination sequence Gal.5.247.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-7) - a(n-8) + a(n-9) -
 *            a(n-10) for n >= 15, with initial values a(0)=1, a(1)=4, a(2)=9,
 *            a(3)=14, a(4)=18, a(5)=22, a(6)=28, a(7)=33, a(8)=37, a(9)=41,
 *            a(10)=47, a(11)=52, a(12)=55, a(13)=60, a(14)=65
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static size_t dp_size = 0;

    /* Create or grow the cache to hold terms up to n. */
    if (dp == NULL) {
        dp_size = (size_t)(n + 1);
        if (dp_size < 15) dp_size = 15;
        dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
        if (!dp) return (seq_t)0; /* allocation failed */

        /* Initialize the first 15 terms. */
        static const seq_t init_vals[15] = {
            (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)14, (seq_t)18,
            (seq_t)22, (seq_t)28, (seq_t)33, (seq_t)37, (seq_t)41,
            (seq_t)47, (seq_t)52, (seq_t)55, (seq_t)60, (seq_t)65
        };
        for (size_t i = 0; i < 15; ++i) dp[i] = init_vals[i];
        for (size_t i = 15; i < dp_size; ++i) dp[i] = (seq_t)0;
    } else if ((size_t)(n + 1) > dp_size) {
        dp_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, dp_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        dp = tmp;
        for (size_t i = 15; i < dp_size; ++i) dp[i] = (seq_t)0;
    }

    /* Compute terms that have not been computed yet. */
    for (size_t i = 15; i <= (size_t)n; ++i) {
        if (dp[i] != (seq_t)0) continue; /* already set */
        dp[i] = dp[i-1] - dp[i-2] + dp[i-3] + dp[i-7]
                - dp[i-8] + dp[i-9] - dp[i-10];
    }

    return dp[(size_t)n];
}
