/*
 * A379505  a(n) is the number of ways of partitioning the divisors of n into
 *          two disjoint sets with equal sum, when an extra 1-divisor is added
 *          to the divisor set, and the two 1-divisors are considered
 *          indistinguishable.
 * https://oeis.org/A379505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1799, u32 n <= 1799, u64 n <= 1799,
 *            u128 n <= 1799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ways to partition the divisors of n, with an extra
 * 1 added to the divisor set, into two disjoint sets of equal sum.
 * The two 1‑divisors are considered indistinguishable.
 * Algorithm: dynamic–programming subset sum on all divisors >1.
 * The multiplicity‑2 1’s are handled by adding 0, 1, or 2 of them
 * after the DP.  For n>1 the answer is (number of subsets)/2;
 * for n=1 it is 1.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1) return (seq_t)1;

    /* Collect divisors >1 and compute σ(n) */
    idx_t divisors[128];
    size_t divcnt = 0;
    seq_t sigma = (seq_t)0;

    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i, d2 = n / i;

            sigma += (seq_t)d1;
            if (d1 > 1) divisors[divcnt++] = d1;

            if (d2 != d1) {
                sigma += (seq_t)d2;
                if (d2 > 1) divisors[divcnt++] = d2;
            }
        }
    }

    /* Total sum after adding the extra 1 */
    seq_t total = sigma + (seq_t)1;

    /* If the sum is odd, no equal‑sum partition exists */
    if ((total % (seq_t)2) != (seq_t)0) return (seq_t)0;

    /* Target sum for each side */
    seq_t target = total / (seq_t)2;
    size_t tgt = (size_t)target;        /* fits into size_t for n ≤ 10⁴ */

    /* DP array: dp[s] = number of ways to reach sum s using divisors >1 */
    seq_t *dp = (seq_t*)calloc(tgt + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;
    dp[0] = (seq_t)1;

    /* Subset sum over divisors >1 (each used at most once) */
    size_t j;
    for (j = 0; j < divcnt; ++j) {
        idx_t val = divisors[j];
        for (idx_t s = tgt; s >= val; --s) {
            dp[s] += dp[s - val];
            if (s == val) break;       /* avoid unsigned wrap around */
        }
    }

    /* Count subsets that sum to target when adding 0, 1, or 2 ones */
    seq_t ways = dp[tgt];
    if (tgt >= 1) ways += dp[tgt - 1];
    if (tgt >= 2) ways += dp[tgt - 2];

    free(dp);

    /* Each unordered partition corresponds to two subsets, hence divide by 2 */
    return ways / (seq_t)2;
}
