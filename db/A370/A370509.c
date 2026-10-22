/*
 * A370509  Expansion of Sum_{k>=0} k! * ( x * (1+x^2) )^k.
 * https://oeis.org/A370509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-2*k)! * binomial(n-2*k, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;            /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m   = n - 2 * k;     /* m = n - 2*k  */
        seq_t term = factorial(m) * binomial(m, k);
        sum += term;
    }
    return sum;
}
