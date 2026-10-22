/*
 * A307618  A Calabi-Yau period integral: a(n) = C(4*n,2*n)*C(2*n,n)^3.
 * https://oeis.org/A307618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4*n, 2*n) * C(2*n, n)^3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute the two binomial coefficients */
    seq_t c1 = binomial(4 * n, 2 * n);   /* C(4n, 2n) */
    seq_t c2 = binomial(2 * n, n);       /* C(2n, n) */

    /* compute C(2n, n)^3 */
    seq_t c2_cubed = seq_pow(c2, 3);

    /* return the product */
    return seq_mul(c1, c2_cubed);
}
