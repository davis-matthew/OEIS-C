/*
 * A361713  a(n) = Sum_{k = 0..n-1} binomial(n,k)^2 * binomial(n+k-1,k)^2.
 * https://oeis.org/A361713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n-1} binomial(n,k)^2 * binomial(n+k-1,k)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <assert.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* If n == 0, the loop body will not be executed and sum stays 0. */
    seq_t c1 = 1;    /* binomial(n,0)   */
    seq_t c2 = 1;    /* binomial(n-1,0) */

    for (idx_t k = 0; k < n; ++k)
    {
        /* term = binomial(n,k)^2 * binomial(n+k-1,k)^2 */
        seq_t term1 = c1 * c1;   /* (binomial(n,k))^2   */
        seq_t term2 = c2 * c2;   /* (binomial(n+k-1,k))^2 */
        sum += term1 * term2;

        /* Update the binomials for the next k, unless we are at the last one */
        if (k + 1 < n)
        {
            seq_t denom = (seq_t)(k + 1);
            seq_t num1  = (seq_t)(n - k);   /* for binomial(n,k+1)   */
            seq_t num2  = (seq_t)(n + k);   /* for binomial(n+k, k+1) */

            c1 = (c1 * num1) / denom;   /* binomial(n,k+1) = binomial(n,k) * (n-k)/(k+1) */
            c2 = (c2 * num2) / denom;   /* binomial(n+k, k+1) = binomial(n+k-1,k) * (n+k)/(k+1) */
        }
    }

    return sum;
}
