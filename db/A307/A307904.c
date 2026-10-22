/*
 * A307904  Coefficient of x^n in (1 + x + n*x^3)^n.
 * https://oeis.org/A307904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} n^k * binomial(n,3k) * binomial(3k,k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t pow_nk = pow_int((seq_t)n, k);          /* n^k */
        seq_t term   = pow_nk * binomial(n, 3 * k);   /* n^k * C(n,3k) */
        term          = term * binomial(3 * k, k);    /* * C(3k,k) */
        sum += term;
    }
    return sum;
}
