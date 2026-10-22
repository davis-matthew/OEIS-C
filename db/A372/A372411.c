/*
 * A372411  Coefficient of x^n in the expansion of ( (1-x+x^2)^2 / (1-x)^3
 *          )^n.
 * https://oeis.org/A372411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*n,k) * binomial(2*n-k-1,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* a(0) = 1 */

    idx_t maxk = n / 2;                     /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(2*n, k);                               /* C(2*n, k)      */
        seq_t term2 = binomial(2*n - k - 1, n - 2*k);                 /* C(2*n-k-1, n-2k) */
        sum += term1 * term2;
    }

    return sum;
}
