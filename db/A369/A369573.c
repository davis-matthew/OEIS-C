/*
 * A369573  Expansion of Product_{k>=1} (1 + x^(k^2)) / (1 - x^(k^3)).
 * https://oeis.org/A369573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Product_{k>=1} (1 + x^(k^2)) / (1 -
 *            x^(k^3))
 * Range:     u8 n <= 61, u16 n <= 268, u32 n <= 1450, u64 n <= 9488,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming with
   - distinct squares (1 + x^{k^2}) -> bounded by 1
   - unrestricted cubes (1 - x^{k^3})^{-1} -> unbounded use
   The algorithm fills an array dp[0..n] of seq_t
   and returns dp[n].
   It preserves results for the largest n requested
   to avoid recomputation.   */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *dp = NULL;          /* array of coefficients */
    static size_t dp_cap = 0;         /* allocated capacity */
    static idx_t dp_size = 0;         /* number of valid coefficients (dp_size = current n + 1) */

    /* Ensure we have capacity for n+1 elements */
    if (n + 1 > dp_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) /* failed allocation */ abort();
        dp = tmp;
        dp_cap = new_cap;
    }

    /* Recompute only if we need a larger n than previously computed */
    if (n >= dp_size) {
        idx_t N = n;

        /* Initialize dp */
        for (idx_t i = 0; i <= N; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Process distinct squares */
        for (idx_t k = 1; ; ++k) {
            idx_t s = k * k;          /* square value */
            if (s > N) break;

            for (idx_t i = N; ; --i) {
                dp[i] += dp[i - s];
                if (i == s) break;    /* avoid unsigned underflow */
            }
        }

        /* Process unrestricted cubes */
        for (idx_t k = 1; ; ++k) {
            idx_t c = k * k * k;      /* cube value */
            if (c > N) break;
            for (idx_t i = c; i <= N; ++i) {
                dp[i] += dp[i - c];
            }
        }

        dp_size = N + 1;
    }

    return dp[n];
}
