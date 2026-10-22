/*
 * A369873  a(n) is the constant term in the expansion of Product_{d|n} (x^d +
 *          1/x^d).
 * https://oeis.org/A369873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to Σ_{d|n} c_i · d = 0 with c_i ∈
 *            {−1,1}; equivalently, the number of subsets of the divisors of n
 *            whose sum equals half the sum of all divisors
 * Range:     u8 n <= 119, u16 n <= 719, u32 n <= 2519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Gather all divisors of n.
       For n ≤ 10000 τ(n) ≤ 256, so this buffer is sufficient. */
    idx_t divs[256];
    size_t div_cnt = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[div_cnt++] = d;
            idx_t e = n / d;
            if (e != d) divs[div_cnt++] = e;
        }
    }

    /* Sum of all divisors. */
    unsigned long long sigma = 0;
    for (size_t i = 0; i < div_cnt; ++i)
        sigma += divs[i];

    /* If the sum is odd, no subset can reach half of it. */
    if (sigma & 1ULL) return 0;

    unsigned long long target = sigma / 2ULL;
    size_t tgt = (size_t)target;

    /* Subset‑sum DP counting the number of subsets that reach 'target'. */
    unsigned long long *dp = (unsigned long long *)malloc((tgt + 1) * sizeof(unsigned long long));
    if (!dp) return 0;                     /* allocation failure falling back to 0 */

    for (size_t i = 0; i <= tgt; ++i) dp[i] = 0;
    dp[0] = 1ULL;

    for (size_t i = 0; i < div_cnt; ++i) {
        size_t d = (size_t)divs[i];
        for (size_t s = tgt; s >= d; --s)
            dp[s] += dp[s - d];
    }

    seq_t result = (seq_t)dp[tgt];
    free(dp);
    return result;
}
