/*
 * A307903  Coefficient of x^n in (1 + n*x + n*x^3)^n.
 * https://oeis.org/A307903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} n^(n-2*k) * binomial(n,3*k) *
 *            binomial(3*k,k) Computes the coefficient of x^n in (1 + n*x +
 *            n*x^3)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t term = big_pow((seq_t)n, (seq_t)(n - 2*k));
        term *= binomial(n, 3*k);
        term *= binomial(3*k, k);
        sum += term;
    }
    return sum;
}
