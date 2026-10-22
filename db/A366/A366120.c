/*
 * A366120  Number of ways to choose a multiset of values whose sum is n from
 *          the first floor(1+log_2(n)) rows of Pascal's triangle.
 * https://oeis.org/A366120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of subsets of Pascal triangle entries in rows
 *            0..floor(log2(n)) with weighted sum equal to n. Computed by
 *            subset‑sum dynamic programming over all binomial coefficients in
 *            those rows
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 255, u64 n <= 2047,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Count how many bits in idx_t */
static unsigned floor_log2_uint64(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Compute binomial coefficient C(n,k) using integer arithmetic.
   Works for any seq_t (built‑in or big integer classes). */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t K = floor_log2_uint64(n);          /* rows 0..K inclusive */
    size_t size = (size_t)n + 1;

    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0; /* allocation failure, return 0 as safe default */

    for (size_t i = 0; i < size; ++i)
        dp[i] = 0;
    dp[0] = (seq_t)1;

    for (idx_t k = 0; k <= K; ++k)
    {
        for (idx_t j = 0; j <= k; ++j)
        {
            seq_t w_seq = binom(k, j);
            idx_t w = (idx_t)w_seq;
            if (w > n) continue;
            for (idx_t s = n; s >= w; --s)
            {
                dp[s] = dp[s] + dp[s - w];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
