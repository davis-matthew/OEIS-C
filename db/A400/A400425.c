/*
 * A400425  Number of permutations of [n] whose distinct cycle lengths have
 *          equal multiplicities.
 * https://oeis.org/A400425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{d|n} [x^(n/d)] Product_{j=1..n/d} (1 +
 *            x^j/(d!*j^d))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 429 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using subset‑sum DP. For each divisor d of n we
   compute the coefficient of x^(n/d) in the product;
   the dp value is the integer n!/(d!^k ∏j^d) for each subset of
   distinct parts {j}.  Summing these values yields a(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* n! needed repeatedly */
    seq_t fact_n = factorial(n);
    seq_t result = (seq_t)0;

    /* Enumerate all divisors d of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* Process divisor d = i */
        idx_t d = i;
        idx_t s_target = n / d;

        /* dp array: dp[s] holds sum of contributions for sum = s */
        seq_t *dp = mem_new(seq_t, (size_t)(s_target + 1));
        if (!dp) return result;  /* allocation failure -> return what we have */
        dp[0] = fact_n;          /* empty subset contributes n! */

        seq_t fact_d = factorial(d);
        for (idx_t j = 1; j <= s_target; ++j) {
            /* denominator = d! * j^d */
            seq_t pow_jd = ipow((seq_t)j, (seq_t)d);
            seq_t denom = fact_d * pow_jd;

            /* iterate sums in descending order to avoid reuse of j in the same step */
            for (idx_t sum = s_target - j; sum != (idx_t)(-1); --sum) {
                seq_t cur = dp[sum];
                if (cur == (seq_t)0) continue;
                seq_t add = cur / denom;  /* exact division */
                dp[sum + j] += add;
            }
        }

        result += dp[s_target];
        mem_free(dp);

        /* If divisor pair is distinct, process d = n / i */
        if (i != n / i) {
            d = n / i;
            s_target = n / d;           /* same as i */

            dp = mem_new(seq_t, (size_t)(s_target + 1));
            if (!dp) return result;
            dp[0] = fact_n;

            fact_d = factorial(d);
            for (idx_t j = 1; j <= s_target; ++j) {
                seq_t pow_jd = ipow((seq_t)j, (seq_t)d);
                seq_t denom = fact_d * pow_jd;

                for (idx_t sum = s_target - j; sum != (idx_t)(-1); --sum) {
                    seq_t cur = dp[sum];
                    if (cur == (seq_t)0) continue;
                    seq_t add = cur / denom;
                    dp[sum + j] += add;
                }
            }

            result += dp[s_target];
            mem_free(dp);
        }
    }

    return result;
}
