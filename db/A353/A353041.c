/*
 * A353041  G.f. A(x) satisfies: A(x) = 1 + x * A(3*x/(1 + 2*x)) / (1 - x).
 * https://oeis.org/A353041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-1,k-1) * 3^{k*(k-1)/2}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* N = n-1 for binomial arguments */
    idx_t N = n - 1;
    seq_t total = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t kk = k - 1;                     /* binomial(n-1,k-1) */
        seq_t bin = binomial(N, kk);

        /* exponent e = k*(k-1)/2 calculated on seq_t type */
        seq_t exp = (seq_t)k * ((seq_t)k - 1) / 2;
        seq_t term = bin * big_pow((seq_t)3, exp);

        total += term;
    }

    return total;
}
