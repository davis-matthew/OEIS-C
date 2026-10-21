/*
 * A285245  Expansion of Product_{k>=1} 1/(1 - k*x^(k^2)).
 * https://oeis.org/A285245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 - k * x^{k^2})
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 117, u64 n <= 245,
 *            u128 n <= 501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Use dynamic array for DP.  The array holds all values up to the
   largest n requested so far. */
static seq_t *dp = NULL;
static size_t dp_cap = 0;

/* Ensure the dp array has room for indices up to n.  Any new
   elements are zeroed. */
static void ensure_dp_cap(idx_t n)
{
    if (dp_cap <= n) {
        size_t new_cap = dp_cap ? dp_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        dp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        dp_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Set up DP array */
    ensure_dp_cap(n);
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;

    /* Compute DP.  For each k, add contributions of using k squares. */
    const seq_t seqt_max = (seq_t)-1;            /* maximum value representable by seq_t */

    for (idx_t k = 1; k * k <= n; ++k) {
        idx_t weight = k * k;
        for (idx_t t = weight; t <= n; ++t) {
            seq_t prod;
            if (sizeof(seq_t) <= 8) {
                unsigned __int128 prod128 = (unsigned __int128)dp[t - weight] * (unsigned long long)k;
                if (prod128 > (unsigned __int128)seqt_max)
                    prod128 = (unsigned __int128)seqt_max;
                prod = (seq_t)prod128;

                unsigned __int128 sum128 = (unsigned __int128)dp[t] + prod128;
                dp[t] = sum128 > (unsigned __int128)seqt_max
                        ? seqt_max
                        : (seq_t)sum128;
            } else {
                /* For arbitrary‑precision seq_t, overflow is handled by the type itself. */
                dp[t] += dp[t - weight] * (seq_t)k;
            }
        }
    }
    return dp[n];
}
