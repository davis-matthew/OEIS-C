/*
 * A341904  Number of representations of n as a linear combination of positive
 *          Fibonacci numbers with coefficients {0,1,2} only.
 * https://oeis.org/A341904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as Σ e_i·F_i with
 *            e_i∈{0,1,2} and the Fibonacci numbers F_i = 1,2,3,5,… (F_1=1,
 *            F_2=2)
 * Range:     u8 n <= 182, u16 n <= 9394, u32 n <= 9394, u64 n <= 9394,
 *            u128 n <= 9394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1.  Use malloc to be compatible with C and C++. */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;        /* allocation failed */

    /* Initialise the DP table. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;           /* one way to represent 0 */

    /* Build the list of Fibonacci numbers not exceeding n. */
    idx_t fibs[100];            /* < 100 for n ≤ 10^19, safe for n ≤ 10000 */
    size_t fib_cnt = 0;
    idx_t a_fib = 1, b_fib = 2;
    while (a_fib <= n) {
        fibs[fib_cnt++] = a_fib;
        idx_t tmp = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = tmp;
    }

    /* Bounded coin‑change DP: each Fibonacci coin may be used 0, 1 or 2 times. */
    for (size_t k = 0; k < fib_cnt; ++k) {
        idx_t f = fibs[k];
        for (idx_t s = n; s >= f; --s) {
            dp[s] += dp[s - f];            /* use one copy of f */
            if (s >= 2 * f)
                dp[s] += dp[s - 2 * f];    /* use two copies of f */
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
