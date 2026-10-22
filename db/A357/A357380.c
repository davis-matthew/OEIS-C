/*
 * A357380  Expansion of Product_{k>=1} (1 - x^Fibonacci(k)).
 * https://oeis.org/A357380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n in the infinite product * ∏_{k≥1}
 *            (1 - x^{Fibonacci(k)}), * i.e. the signed count of
 *            representations of n as a sum * of distinct Fibonacci numbers,
 *            where the Fibonacci sequence * starts 1,1,2,3,5,... * *
 *            Computation: dynamic programming over the Fibonacci factors.
 *            *...
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array for coefficients up to n */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* allocation failure: return 0 as a safe default */

    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;  /* empty sum contributes +1 */

    /* first Fibonacci number 1 (two identical factors) */
    for (int i = (int)n; i >= 1; --i) dp[i] -= dp[i - 1];
    /* second factor 1 */
    for (int i = (int)n; i >= 1; --i) dp[i] -= dp[i - 1];

    /* process the remaining Fibonacci numbers: 2, 3, 5, … */
    idx_t prev1 = 1; /* second last processed Fibonacci number */
    idx_t prev2 = 1; /* last    processed Fibonacci number */
    while (1) {
        idx_t next = prev1 + prev2;        /* next Fibonacci number */
        if (next > n) break;               /* stop if it exceeds n */
        for (int i = (int)n; i >= (int)next; --i) {
            dp[i] -= dp[i - next];
        }
        prev2 = prev1;
        prev1 = next;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
