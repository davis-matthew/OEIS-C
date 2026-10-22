/*
 * A389833  a(n) = n! * [x^n] exp(x*(1 + n*exp(x))).
 * https://oeis.org/A389833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * n^k * (k+1)^(n-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binomial(n, k);
        term *= seq_pow((seq_t)n, k);
        term *= seq_pow((seq_t)(k + 1), n - k);
        sum += term;
    }
    return sum;
}
