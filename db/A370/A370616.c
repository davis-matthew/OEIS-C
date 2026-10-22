/*
 * A370616  Coefficient of x^n in the expansion of ( (1-x) / (1-x-x^2) )^n.
 * https://oeis.org/A370616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n+k-1,k) * binomial(n-k-1,n-2k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle small n directly to avoid negative binomial arguments. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;

    seq_t sum = (seq_t)0;
    idx_t maxk = n >> 1;      /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k - (idx_t)1, k);
        seq_t t2 = binomial(n - k - (idx_t)1, n - (idx_t)2 * k);
        sum += t1 * t2;
    }
    return sum;
}
