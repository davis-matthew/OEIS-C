/*
 * A285044  Expansion of cosh(5*arctanh(2*sqrt(x))).
 * https://oeis.org/A285044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(64*n^2 + 8*n + 3)*binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* Compute binomial(2n,n) */
    seq_t binom = binomial(2 * n, n);

    /* Compute polynomial poly = 64*n^2 + 8*n + 3 */
    seq_t nn = (seq_t)n;
    seq_t poly = 64 * nn * nn + 8 * nn + 3;

    /* Remove all factors of 3 from numerator and denominator
       to avoid overflow before multiplication.            */
    seq_t g1 = gcd(poly, (seq_t)3);
    seq_t poly1 = poly / g1;
    seq_t div1 = 3 / g1;          /* div1 is 1 or 3 */

    seq_t g2 = gcd(binom, div1);
    seq_t binom1 = binom / g2;
    seq_t div2 = div1 / g2;       /* div2 should be 1 */

    (void)div2;                   /* silence unused-variable warning */

    /* Final result: (binom1 * poly1) */
    return binom1 * poly1;
}
