/*
 * A292830  a(1) = 1, for n>=2, a(n) = B(2*n-1, n), where B(n, x) is the
 *          Bernoulli polynomial.
 * https://oeis.org/A292830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n - 1) * (1^(2*n-2) + 2^(2*n-2) + ... + (n-1)^(2*n-2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t exp   = (seq_t)(2 * (seq_t)n - 2);
    seq_t scale = (seq_t)(2 * (seq_t)n - 1);

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t term = big_pow((seq_t)k, exp);
        sum += term;
    }

    return scale * sum;
}
