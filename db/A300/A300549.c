/*
 * A300549  a(n) = [x^n] Product_{d|n} (1 + x^d)/(1 - x^d).
 * https://oeis.org/A300549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{d | n} (1 + x^d)/(1 - x^d)
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over the divisors of n. */
/* Each divisor d contributes the factor 1 + 2·x^d + 2·x^{2d} + … .    */
/* The coefficient for exponent t after processing a divisor d is    */
/* newdp[t] = olddp[t] + 2·∑_{k≥1} olddp[t − k·d], which can be    */
/* evaluated by summing over each residue class modulo d.           */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP array of size n+1.  Use malloc to stay generic. */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* memory failure – unlikely in test stack. */
    for (size_t i = 0; i <= N; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Enumerate all positive divisors of n.  The array size is ample for n up to 1e9. */
    idx_t divs[2048];
    size_t divCnt = 0;
    for (idx_t i = 1; (i * i) <= n; ++i) {
        if (n % i == 0) {
            divs[divCnt++] = i;
            if (i != n / i)
                divs[divCnt++] = n / i;
        }
    }

    /* Process each divisor.  For each residue class modulo d we
       accumulate contributions from multiples of d. */
    for (size_t di = 0; di < divCnt; ++di) {
        idx_t d = divs[di];
        for (idx_t r = 0; r < d; ++r) {
            seq_t sum = 0;
            for (idx_t t = r; t <= n; t += d) {
                seq_t old_val = dp[t];
                dp[t] = old_val + ((seq_t)2) * sum;
                sum += old_val;
            }
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
