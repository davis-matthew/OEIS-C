/*
 * A359176  a(n) = binomial(2*n-2,n-1) - n.
 * https://oeis.org/A359176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-2, n-1) - n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Multiplicative binomial coefficient that keeps intermediate values
   as small as possible.  It works with any seq_t that supports + - * / and
   gcd(seq_t, seq_t). */
static seq_t binomial_multiplicative(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= K; ++i) {
        seq_t num = N - K + i;   /* numerator part */
        seq_t den = i;           /* denominator part */

        /* Reduce result and denominator first */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* Reduce numerator and remaining denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* Now multiply (den should be 1) */
        result = result * num;
        if (den != (seq_t)1)   /* safety check for any remaining factor */
            result /= den;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */
    seq_t bin = binomial_multiplicative((idx_t)(2 * n - 2), (idx_t)(n - 1));
    return bin - n;
}
