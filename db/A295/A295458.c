/*
 * A295458  a(n) = (30*n)!*(5*n)!*(4*n)!/((15*n)!*(10*n)!*(8*n)!*(6*n)!).
 * https://oeis.org/A295458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(30*n,15*n)*binomial(15*n,5*n)*binomial(4*n,n)/
 *            (binomial(8*n,3*n)*binomial(6*n,n))
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute numerator and denominator in small pieces.
       The division is exact by definition of the sequence. */
    idx_t t30 = 30 * n;
    idx_t t15 = 15 * n;
    idx_t t5  = 5  * n;
    idx_t t4  = 4  * n;
    idx_t t3  = 3  * n;
    idx_t t6  = 6  * n;

    seq_t num = binomial(t30, t15);
    num = num * binomial(t15, t5);
    num = num * binomial(t4, n);

    seq_t den = binomial(8 * n, t3);
    den = den * binomial(t6, n);

    return num / den;
}
