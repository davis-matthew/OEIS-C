/*
 * A339619  Number of integer partitions of n with no 1's and a part divisible
 *          by all the other parts.
 * https://oeis.org/A339619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{L=2..n} p_div(L, n-L) where p_div(L, m) is the
 *            number of partitions of m into parts that divide L. (All
 *            partitions use parts ≥2 and include at least one part equal to
 *            L.)
 * Range:     u8 n <= 41, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *
compute_table(idx_t N)
{
    /* Allocate array for a[0..N] and initialize. */
    seq_t *a = mem_new(seq_t, (size_t)(N + 1));
    for (idx_t i = 0; i <= N; ++i) {
        a[i] = (seq_t)0;
    }
    a[0] = (seq_t)1;                /* a(0) = 1 */

    /* Temporary DP array reused for each L. */
    seq_t *dp = mem_new(seq_t, (size_t)(N + 1));

    for (idx_t L = 2; L <= N; ++L) {
        idx_t max_sum = N - L;
        /* Zero dp[0..max_sum] for this L. */
        for (idx_t i = 0; i <= max_sum; ++i) {
            dp[i] = (seq_t)0;
        }
        dp[0] = (seq_t)1;  /* empty partition */

        /* Add contributions from each divisor d of L (d >= 2). */
        for (idx_t d = 2; d * d <= L; ++d) {
            if (L % d == 0) {
                /* divisor d */
                for (idx_t s = d; s <= max_sum; ++s) {
                    dp[s] = dp[s] + dp[s - d];
                }
                /* paired divisor L/d */
                idx_t d2 = L / d;
                if (d2 != d) {
                    for (idx_t s = d2; s <= max_sum; ++s) {
                        dp[s] = dp[s] + dp[s - d2];
                    }
                }
            }
        }
        /* divisor L itself */
        if (L <= max_sum) {
            for (idx_t s = L; s <= max_sum; ++s) {
                dp[s] = dp[s] + dp[s - L];
            }
        }

        /* Accumulate results into a[n] where n = L + s. */
        for (idx_t s = 0; s <= max_sum; ++s) {
            a[L + s] = a[L + s] + dp[s];
        }
    }

    mem_free(dp);
    return a;
}

/* Assume seq_t and idx_t are defined elsewhere. */
static seq_t *memo = NULL;
static idx_t memo_max = 0;
static bool memo_ok = false;

seq_t a(idx_t n)
{
    if (!memo_ok || n > memo_max) {
        seq_t *newtable = compute_table(n);
        if (memo_ok) {
            mem_free(memo);
        }
        memo = newtable;
        memo_max = n;
        memo_ok = true;
    }
    return memo[n];
}
