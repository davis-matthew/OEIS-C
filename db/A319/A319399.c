/*
 * A319399  Number of partitions of n into exactly six positive Fibonacci
 *          numbers.
 * https://oeis.org/A319399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into exactly six positive
 *            Fibonacci numbers. Computed by dynamic programming over the
 *            Fibonacci values
 * Range:     u8 n <= 603, u16 n <= 9930, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;

    /* Generate all positive Fibonacci numbers <= n
       (1,2,3,5,8,13,...).  The starting value 1 occurs only once. */
    seq_t fibs[64];
    size_t m = 0;
    fibs[m++] = (seq_t)1;               /* first Fibonacci number */
    seq_t a_fib = (seq_t)1;              /* F(2) */
    seq_t b_fib = (seq_t)2;              /* F(3) */
    while (b_fib <= (seq_t)n) {
        fibs[m++] = b_fib;
        seq_t c = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = c;
    }

    size_t S = (size_t)n + 1;
    /* Allocate two layers of DP tables: dp[2][7][S] */
    seq_t *dp[2][7];
#if defined(__cplusplus)
    for (size_t k = 0; k <= 6; ++k) {
        dp[0][k] = new seq_t[S];
        dp[1][k] = new seq_t[S];
    }
#else
    for (size_t k = 0; k <= 6; ++k) {
        dp[0][k] = (seq_t*)calloc(S, sizeof(seq_t));
        dp[1][k] = (seq_t*)calloc(S, sizeof(seq_t));
    }
#endif

    /* Initialize dp[0] */
    for (size_t k = 0; k <= 6; ++k)
        for (size_t s = 0; s < S; ++s)
            dp[0][k][s] = (seq_t)0;
    dp[0][0][0] = (seq_t)1;

    int cur = 0, nxt = 1;

    /* Dynamic programming over Fibonacci numbers */
    for (size_t f_idx = 0; f_idx < m; ++f_idx) {
        seq_t f = fibs[f_idx];

        /* Clear next layer */
        for (size_t k = 0; k <= 6; ++k)
            for (size_t s = 0; s < S; ++s)
                dp[nxt][k][s] = (seq_t)0;

        for (size_t k = 0; k <= 6; ++k) {
            for (size_t s = 0; s <= n; ++s) {
                seq_t val = dp[cur][k][s];
                if (val == (seq_t)0) continue;

                /* take 0 copies of f */
                dp[nxt][k][s] += val;

                /* take t copies of f, 1 <= t <= (6-k), staying within sum n */
                if (k < 6 && f <= (seq_t)(n - s)) {
                    seq_t sum_val = val;
                    size_t ns = s + (size_t)f;
                    size_t tmax = 6 - k;
                    for (size_t t = 1; t <= tmax; ++t, ns += (size_t)f) {
                        if (ns > n) break;
                        dp[nxt][k + t][ns] += val;
                    }
                }
            }
        }
        /* swap layers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = dp[cur][6][n];

    /* Free memory */
    for (size_t k = 0; k <= 6; ++k) {
#if defined(__cplusplus)
        delete[] dp[0][k];
        delete[] dp[1][k];
#else
        free(dp[0][k]);
        free(dp[1][k]);
#endif
    }

    return result;
}
