/*
 * A363433  Number of (123,231)-avoiding stabilized-interval-free permutations
 *          of size n.
 * https://oeis.org/A363433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 1249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence derived from the generating function of A363433:
 *   a(n) = 2*a(n-2) + a(n-3) - a(n-4) - 2*a(n-5) + a(n-7) - c(n),
 * where c(n) are the coefficients of the numerator
 *   N(x) = x^9 + x^8 - 3*x^6 - 2*x^5 + x^4 + 2*x^3 + x^2 - x - 1
 * (c(0) = -1, c(1)= -1, c(2)= 1, c(3)= 2, c(4)= 1, c(5)= -2,
 *  c(6)= -3, c(7)= 0, c(8)= 1, c(9)= 1, c(n)=0 for n>9).
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=3.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocation of temporary array to hold computed terms up to n. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* fall back on failure */

    /* Numerator coefficients c(0..9).  For n>9 the coefficient is 0. */
    static const int c[10] = { -1, -1,  1,  2,  1,  -2,  -3,   0,   1,   1 };

    for (idx_t i = 0; i <= n; ++i) {
        seq_t rest = 0;

        if (i >= 2) { rest += dp[i - 2]; rest += dp[i - 2]; }      /* 2*a(n-2) */
        if (i >= 3)  rest += dp[i - 3];                           /*   +a(n-3) */
        if (i >= 4)  rest -= dp[i - 4];                           /*   -a(n-4) */
        if (i >= 5) { rest -= dp[i - 5]; rest -= dp[i - 5]; }      /*   -2*a(n-5) */
        if (i >= 7)  rest += dp[i - 7];                           /*   +a(n-7) */

        /* Apply the numerators’ coefficient. */
        seq_t val = rest;
        if (i <= 9) val -= (seq_t)c[i];
        dp[i] = val;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
