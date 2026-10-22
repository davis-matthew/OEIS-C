/*
 * A309679  G.f. A(x) satisfies: A(x) = A(x^5) / (1 - x)^2.
 * https://oeis.org/A309679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x)=A(x^5)/(1-x)^2; recurrence
 *            a(n)=2*a(n-1)-a(n-2)+(n%5==0?a(n/5):0) with a(0)=1
 * Range:     u8 n <= 27, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    size_t len = (size_t)n + 1;

#ifdef __cplusplus
    seq_t *dp = new seq_t[len];
#else
    seq_t *dp = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp) return (seq_t)0;
#endif

    dp[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t rhs = (i % 5 == 0) ? dp[i/5] : 0;
        seq_t cur  = dp[i-1] * 2;
        if (i >= 2) cur -= dp[i-2];
        dp[i] = cur + rhs;
    }

    seq_t result = dp[n];

#ifdef __cplusplus
    delete [] dp;
#else
    free(dp);
#endif

    return result;
}
