/*
 * A329533  First differences of A051924, or second differences of Central
 *          binomial coefficients A000984.
 * https://oeis.org/A329533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n+4, n+2) - 2*binomial(2n+2, n+1) +
 *            binomial(2n, n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* central binomial coefficients */
    seq_t cb0 = binomial(2 * n,          n);   /* C(2n,   n)   */
    seq_t cb1 = binomial(2 * n + 2,    n + 1); /* C(2n+2, n+1) */
    seq_t cb2 = binomial(2 * n + 4,    n + 2); /* C(2n+4, n+2) */

    /* a(n) = cb2 - 2*cb1 + cb0
       we compute as cb2 - cb1, subtract cb1 again, then add cb0
       This avoids underflow because cb2 > 2*cb1 for all n >= 0. */
    seq_t t = cb2 - cb1;
    t -= cb1;
    t += cb0;

    return t;
}
