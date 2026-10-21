/*
 * A245367  Compositions of n into parts 3, 5 and 7.
 * https://oeis.org/A245367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5) + a(n-7) Initial values: a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=1, a(7)=1
 * Range:     u8 n <= 30, u16 n <= 53, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static const seq_t BASE_VALS[8] = {
    (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1,
    (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)1
};

seq_t a(idx_t n)
{
    /* Allocate array of length n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp = seq_alloc_new_array(len);
    if (!dp) return (seq_t)0;   /* In case of allocation failure */

    /* Initialise known base values up to min(n,7) */
    idx_t up = (n < 7) ? n : 7;
    for (idx_t i = 0; i <= up; ++i)
        dp[i] = BASE_VALS[i];

    /* Compute recurrence a(i)=a(i-3)+a(i-5)+a(i-7) for i>=8 */
    for (idx_t i = 8; i <= n; ++i)
        dp[i] = dp[i - 3] + dp[i - 5] + dp[i - 7];

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
