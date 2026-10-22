/*
 * A326850  Number of strict integer partitions of n whose maximum part
 *          divides n.
 * https://oeis.org/A326850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 107, u32 n <= 299, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A326850:
   a(n) = number of strict integer partitions of n whose maximum part divides n.
   For each divisor m of n we count the subsets of {1,…,m‑1} whose sum is n‑m.
   The number of such subsets is the number of partitions of n‑m into distinct
   parts all less than m. */
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */

    /* ---- gather all divisors of n ---- */
    idx_t sq = (idx_t)sqrt((double)n);
    size_t div_capacity = 2 * (sq + 2);
    idx_t *divs = (idx_t *)malloc(div_capacity * sizeof(idx_t));
    size_t div_count = 0;
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i == 0) {
            divs[div_count++] = i;
            if (i != n / i) divs[div_count++] = n / i;
        }
    }

    /* ---- allocation for subset‑sum DP ---- */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t result = 0;

    /* ---- process each divisor ---- */
    for (size_t i = 0; i < div_count; ++i) {
        idx_t m = divs[i];
        idx_t target = n - m;          /* sum we need to attain */
        if (target == 0) {             /* empty partition */
            result += (seq_t)1;
            continue;
        }
        idx_t limit = m - 1;           /* parts must be < m */
        if (limit == 0) continue;

        /* reset dp[0..target] */
        for (idx_t s = 0; s <= target; ++s) dp[s] = 0;
        dp[0] = 1;

        /* classic 0‑1 knapsack over parts 1..limit */
        for (idx_t part = 1; part <= limit; ++part) {
            for (idx_t s = target; s >= part; --s) {
                dp[s] += dp[s - part];
            }
        }

        result += dp[target];
    }

    free(dp);
    free(divs);
    return result;
}
