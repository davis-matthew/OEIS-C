/*
 * A264416  G.f.: Sum_{n>=0} x^n * (d/dx)^(n^2) x^(n^2) * (1+x)^n / (n^2)!,
 *          where (d/dx)^k denotes the k-th derivative operator.
 * https://oeis.org/A264416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k, k) * binomial((n-k)^2 + k,
 *            k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient for indices n,k.
   Works for any seq_t that supports +, -, *, /, =, <.        */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        r = r * ((seq_t)(n - k + i)) / ((seq_t)i);
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / (idx_t)2;
    for (idx_t k = (idx_t)0; k <= maxk; ++k)
    {
        idx_t nmk   = n - k;
        seq_t b1    = _binom(nmk, k);
        idx_t m     = nmk * nmk + k;
        seq_t b2    = _binom(m, k);
        sum += b1 * b2;
    }
    return sum;
}
