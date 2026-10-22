/*
 * A351858  a(n) = [x^n] (1 + x + x^2)^(3*n)/(1 + x)^(2*n).
 * https://oeis.org/A351858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(3*n,k) * C(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    idx_t limit = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= limit; ++k)
    {
        seq_t c1 = binomial(3 * n,   k);   /* C(3n, k)   */
        seq_t c2 = binomial(n - k,   k);   /* C(n-k, k)  */
        sum += c1 * c2;
    }
    return sum;
}
