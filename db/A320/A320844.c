/*
 * A320844  Expansion of Product_{k>0} (1-x^p(k)), where p(k) is the number of
 *          partitions of k (A000041).
 * https://oeis.org/A320844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1 - x^{p(k)})
 * Range:     i8 n <= 859, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute all partition numbers p(k) that are ≤ n */
    size_t part_cap = 64;
    seq_t *part = (seq_t *)malloc(part_cap * sizeof(seq_t));
    if (!part) return (seq_t)0;
    part[0] = 1;
    size_t part_len = 1;           /* number of stored partition numbers */

    size_t exp_cap = 64;
    idx_t *exp = (idx_t *)malloc(exp_cap * sizeof(idx_t));
    if (!exp) { free(part); return (seq_t)0; }
    size_t exp_len = 0;            /* number of exponents (partition numbers) */

    for (idx_t k = 1; ; ++k) {
        if (part_len == part_cap) {
            part_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(part, part_cap * sizeof(seq_t));
            if (!tmp) { free(part); free(exp); return (seq_t)0; }
            part = tmp;
        }

        seq_t val = 0;
        for (int m = 1; ; ++m) {
            idx_t pent1 = (idx_t)m * ((idx_t)3 * m - 1) / 2;
            idx_t pent2 = (idx_t)m * ((idx_t)3 * m + 1) / 2;
            if (pent1 > k && pent2 > k) break;

            seq_t sign = (m & 1) ? 1 : -1;
            if (pent1 <= k) val += sign * part[k - pent1];
            if (pent2 <= k) val += sign * part[k - pent2];
        }

        part[part_len++] = val;          /* store p(k) */

        if (val > (seq_t)n) break;       /* further partitions will be larger */

        if (exp_len == exp_cap) {
            exp_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(exp, exp_cap * sizeof(idx_t));
            if (!tmp) { free(part); free(exp); return (seq_t)0; }
            exp = tmp;
        }
        exp[exp_len++] = (idx_t)val;     /* exponent for DP */
    }

    free(part);

    /* dynamic programming for the product coefficients */
    size_t dp_size = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp) { free(exp); return (seq_t)0; }

    for (size_t i = 1; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    for (size_t idx = 0; idx < exp_len; ++idx) {
        idx_t e = exp[idx];
        for (idx_t j = n; j >= e; --j) {
            dp[j] -= dp[j - e];
            if (j == e) break;          /* avoid unsigned wrap-around */
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(exp);
    return result;
}
