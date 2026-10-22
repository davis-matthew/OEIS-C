/*
 * A335088  a(n) = n! * [x^n] exp(Sum_{k=1..n, gcd(n,k) = 1} x^k / k).
 * https://oeis.org/A335088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp( Sum_{k=1..n, gcd(n,k)=1} x^k / k )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle offset */
    if (n == 0) {
        return (seq_t)1;
    }

    /* build list of allowed cycle lengths k (gcd(k,n)=1) */
    idx_t *allowed = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    if (!allowed) exit(EXIT_FAILURE);
    idx_t allowed_cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            allowed[allowed_cnt++] = k;
        }
    }

    /* factorials 0! .. n! */
    seq_t *fact = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!fact) exit(EXIT_FAILURE);
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* DP array for a[0..n] */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) exit(EXIT_FAILURE);
    dp[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t fact_m1 = fact[m - 1];
        for (idx_t i = 0; i < allowed_cnt; ++i) {
            idx_t k = allowed[i];
            if (k > m) break;
            seq_t weight = fact_m1 / fact[m - k];
            sum += dp[m - k] * weight;
        }
        dp[m] = sum;
    }

    seq_t result = dp[n];

    free(dp);
    free(fact);
    free(allowed);

    return result;
}
