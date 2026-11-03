/*
 * A244827  Number of length n 0..3 arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n 0..3 arrays whose every partial sum
 *            S_k * satisfies (2*S_k - 3k)^2 <= 20k (equivalently |S_k - 1.5k|
 *            <= sqrt(5k)). * This is derived from the requirement that each
 *            partial sum be within * two standard deviations of the mean of a
 *            uniform {0,1,2,3} distribution
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 203 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* local helper: return true if the bound holds for a given sum and prefix length k */
static inline int bound_ok(idx_t sum, idx_t k)
{
    long long diff = (long long)sum * 2 - (long long)3 * (long long)k;
    long long lhs  = diff * diff;
    long long rhs  = 20LL * (long long)k;
    return lhs <= rhs;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1; a(1) is defined */

    /* maximum possible sum is 3*n, we allocate one more for inclusive loops */
    size_t maxSum = (size_t)3 * (size_t)n;
    size_t arraySize = maxSum + 1;

    /* allocate two working arrays, zeroed */
    seq_t *dp   = mem_new(seq_t, arraySize);
    seq_t *next = mem_new(seq_t, arraySize);
    if (!dp || !next)
        abort();                     /* allocation failure */

    /* initialise for k = 1 (prefix of length 1) */
    for (size_t v = 0; v <= 3; ++v)
    {
        if (bound_ok(v, 1))
            dp[v] = (seq_t)1;
    }

    /* DP for k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k)
    {
        size_t limit = (size_t)3 * (size_t)k;      /* sums up to 3k */
        /* reset next array up to new limit */
        mem_set(next, 0, (limit + 1) * sizeof(seq_t));

        for (size_t s = 0; s <= limit; ++s)
        {
            if (!bound_ok(s, k))
                continue;           /* this prefix sum is invalid */

            /* accumulate contributions from previous step */
            seq_t sum = dp[s];          /* last element 0 */
            if (s >= 1) sum += dp[s-1];
            if (s >= 2) sum += dp[s-2];
            if (s >= 3) sum += dp[s-3];

            next[s] = sum;
        }

        /* swap dp and next for the next iteration */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    /* final answer: sum over all valid sums for length n */
    seq_t ans = 0;
    for (size_t s = 0; s <= maxSum; ++s)
        ans += dp[s];

    /* free memory */
    mem_free(dp);
    mem_free(next);

    return ans;
}
