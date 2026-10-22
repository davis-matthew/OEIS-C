/*
 * A374080  Expansion of Product_{k>=1} (1 - x^(4*k-1)) * (1 - x^(4*k)).
 * https://oeis.org/A374080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in \prod_{k>=1} (1 - x^(4k-1)) (1 -
 *            x^(4k))
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *dp = NULL;
static size_t dp_cap = 0;

/* Ensure dp has space for indices up to n (inclusive). */
static void ensure_dp(size_t n)
{
    if (dp_cap > n)
        return;
    size_t new_cap = n + 1;
    seq_t *p = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!p)
        abort();
    dp = p;
    dp_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    ensure_dp(N);

    /* initialise dp[0..N] to zero, then set dp[0]=1 */
    for (size_t i = 0; i <= N; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* iterate over allowed exponents m (≡0 or 3 mod 4, m≥3) */
    for (size_t m = 3; m <= N; ++m)
    {
        if ((m & 3) == 0 || (m & 3) == 3)
        {
            for (size_t k = N; k >= m; --k)
                dp[k] -= dp[k - m];
        }
    }

    return dp[N];
}
