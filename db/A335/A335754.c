/*
 * A335754  a(n) is the number of overpartitions of n where overlined parts
 *          are not divisible by 3 and non-overlined parts are congruent to 1
 *          modulo 3.
 * https://oeis.org/A335754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of overpartitions of n * where overlined parts are
 *            not divisible by 3 * and non‑overlined parts are ≡1 (mod 3). *
 *            G.f. = ∏_{k≥1} (1+q^{3k-1}) * (1+q^{3k-2})/(1−q^{3k-2})
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate coefficient arrays */
    seq_t *dp  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp || !tmp) {
        free(dp); free(tmp);
        return (seq_t)0;          /* unlikely failure */
    }

    /* Initial series: 1 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    for (idx_t s = 1; s <= n; ++s) {
        if (s % 3 == 1) {          /* factor (1+q^s)/(1−q^s) = 1 + 2∑_{m≥1} q^{m s} */
            for (idx_t r = 0; r < s; ++r) {
                seq_t sum = (seq_t)0;
                for (idx_t j = r; j <= n; j += s) {
                    tmp[j] = dp[j] + (seq_t)2 * sum;
                    sum += dp[j];
                }
            }
            /* swap dp and tmp */
            seq_t *swap = dp;
            dp = tmp;
            tmp = swap;            /* tmp now holds old dp values */
        } else if (s % 3 == 2) {   /* factor (1+q^s) */
            for (idx_t i = n; i >= s; --i) {
                dp[i] += dp[i - s];
            }
        }                           /* s % 3 == 0 : nothing to do */
    }

    seq_t result = dp[n];
    free(dp);
    free(tmp);
    return result;
}
