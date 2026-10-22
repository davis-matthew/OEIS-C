/*
 * A372371  Coefficient of x^n in the expansion of ( (1+x+x^3)^2 / (1+x) )^n.
 * https://oeis.org/A372371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n,k) * binomial(n-k,
 *            n-3*k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;     /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t two_n = n * ((idx_t)2);
        seq_t term1 = binomial(two_n, k);
        seq_t term2 = binomial(n - k, n - 3 * k);
        sum += term1 * term2;
    }
    return sum;
}
