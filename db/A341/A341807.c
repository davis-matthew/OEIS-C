/*
 * A341807  Number of ways to write n as an ordered sum of 8 nonzero
 *          tetrahedral numbers.
 * https://oeis.org/A341807
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 8‑tuples of nonzero tetrahedral numbers
 *            summing to n, i.e. coefficient of x^n in (∑_{k≥1}
 *            x^{C(k+2,3)})^8
 * Range:     u8 n <= 25, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 8 the answer is zero (offset 8).  The code runs
       for any n ≥ 0 but returns 0 for n < 8. */
    if (n < 8)
        return (seq_t)0;

    /* Allocate DP array of size n+1, zeroed. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;
    dp[0] = (seq_t)1;

    /* Build list of tetrahedral numbers <= n. */
    idx_t tlist[128];
    size_t tcnt = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t t = (k * (k + 1) * (k + 2)) / 6;
        if (t > n)
            break;
        tlist[tcnt++] = t;
    }

    /* Perform 8 successive convolutions, counting ordered sums. */
    for (size_t step = 0; step < 8; ++step) {
        seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!next) {
            free(dp);
            return (seq_t)0;
        }
        for (idx_t s = 0; s <= n; ++s) {
            seq_t cur = dp[s];
            if (!cur)
                continue;
            for (size_t i = 0; i < tcnt; ++i) {
                idx_t ns = s + tlist[i];
                if (ns > n)
                    break;
                next[ns] += cur;
            }
        }
        free(dp);
        dp = next;
    }

    seq_t ans = dp[(size_t)n];
    free(dp);
    return ans;
}
