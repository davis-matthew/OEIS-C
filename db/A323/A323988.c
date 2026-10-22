/*
 * A323988  a(n) = 2^(n - 1) + binomial(n, floor(n/2))*(n + 1)/2.
 * https://oeis.org/A323988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) + binomial(n, floor(n/2))*(n + 1)/2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   binomial, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* First term: 2^(n-1)  (pow2 accepts an idx_t exponent) */
    seq_t term1 = pow2(n - 1);

    /* Second term: binomial(n, floor(n/2)) * (n + 1) / 2 */
    idx_t k = n / 2;
    seq_t b = binomial(n, k);

    /* Since binomial(n, floor(n/2)) * (n + 1) is always even,
       avoid overflow by dividing one factor by 2 beforehand.           */
    seq_t part;
    if ((n & (idx_t)1) == 0) {            /* n even: n+1 is odd, b is even */
        part = mul_small(b >> 1, (unsigned int)(n + 1));
    } else {                               /* n odd: n+1 is even, b is odd */
        part = mul_small(b, ((unsigned int)(n + 1)) >> 1);
    }

    return term1 + part;
}
