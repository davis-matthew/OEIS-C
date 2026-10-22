/*
 * A321880  Number of partitions of n into colored blocks of equal parts with
 *          colors from a set of size n.
 * https://oeis.org/A321880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{j=1..n} (1 + (n-1)*x^j)/(1 - x^j)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 57, u128 n <= 149,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        seq_t one = 1;
        return one;
    }

    size_t N = (size_t)n + 1;
    seq_t *dp   = alloc_seq_array(N);
    seq_t *newdp = alloc_seq_array(N);
    seq_t *S    = alloc_seq_array(N);

    for (size_t i = 0; i < N; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t j = 1; j <= n; ++j) {
        size_t sj = (size_t)j;
        /* compute prefix sums S[t] = sum_{k>=0} dp[t - k*j] */
        S[0] = dp[0];
        for (size_t t = 1; t < N; ++t) {
            if (t < sj)
                S[t] = dp[t];
            else
                S[t] = dp[t] + S[t - sj];
        }

        /* update dp -> newdp */
        for (size_t t = 0; t < N; ++t) {
            if (t < sj)
                newdp[t] = dp[t];
            else
                newdp[t] = dp[t] + (seq_t)n * S[t - sj];
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[(size_t)n];
    free_seq_array(dp);
    free_seq_array(newdp);
    free_seq_array(S);
    return result;
}
