/*
 * A262952  Expansion of Product_{k>=1} (1 + x^(2*k-1)) * (1 + x^(3*k-1)).
 * https://oeis.org/A262952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^(2*k-1))(1 +
 *            x^(3*k-1))
 * Range:     u8 n <= 34, u16 n <= 93, u32 n <= 284, u64 n <= 948,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* use a simple dynamic programming that multiplies the generating
       function factor by factor.  For each k we first multiply by
       (1 + x^(2k-1)) and then by (1 + x^(3k-1)).  The array dp[s] holds
       the number of ways to obtain a sum s up to the current k. */
    if (n == 0) {
        seq_t v = 1;
        return v;
    }

    idx_t N = n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0; /* allocation failure: return 0 (unlikely) */

    /* initialize */
    dp[0] = 1;
    for (idx_t i = 1; i <= N; ++i) dp[i] = 0;

    for (idx_t k = 1;; ++k) {
        idx_t a = 2 * k - 1;   /* exponent from the odd part sequence */
        idx_t b = 3 * k - 1;   /* exponent from the 2 mod 3 part sequence */

        if (a > N && b > N) break;

        if (a <= N) {
            for (idx_t s = N; s >= a; --s) {
                dp[s] += dp[s - a];
            }
        }
        if (b <= N) {
            for (idx_t s = N; s >= b; --s) {
                dp[s] += dp[s - b];
            }
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
