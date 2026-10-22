/*
 * A350737  Expansion of Sum_{k>=0} (-1)^k * x^(k*(3*k+1)/2) /
 *          Product_{j=1..k} (1-x^j).
 * https://oeis.org/A350737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) are the coefficients in the expansion of Sum_{k>=0} (-1)^k
 *            * x^{k(3k+1)/2} / Product_{j=1..k} (1 - x^j)
 * Range:     i8 n <= 75, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_N 10000
static seq_t dp[MAX_N + 1];

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (int k = 0; ; ++k) {
        idx_t e = (idx_t)k * ((idx_t)3 * k + 1) / 2;
        if (e > n) break;
        idx_t m = n - e;

        /* initialise dp[0..m] */
        for (idx_t i = 0; i <= m; ++i) dp[i] = 0;
        dp[0] = 1;

        /* compute partitions of m into parts <= k */
        for (int part = 1; part <= k; ++part) {
            for (idx_t s = part; s <= m; ++s) {
                dp[s] += dp[s - part];
            }
        }

        seq_t val = dp[m];
        if (k & 1) sum -= val;
        else      sum += val;
    }
    return sum;
}
