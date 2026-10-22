/*
 * A366590  G.f. A(x) satisfies A(x) = 1 + x^2*(1+x)^2*A(x)^3.
 * https://oeis.org/A366590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)}
 *            binomial(2k,n-2k)*binomial(3k,k)/(2k+1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t t = n - 2 * k;
        seq_t c1 = binomial(2 * k, t);
        seq_t c2 = binomial(3 * k, k);
        seq_t denom = (seq_t)(2 * k + 1);

        seq_t g = gcd(c1, denom);
        c1 /= g;
        denom /= g;
        g = gcd(c2, denom);
        c2 /= g;
        denom /= g;                          /* denom should now be 1 */

        seq_t term = c1 * c2;                /* integer division already removed denom */
        sum += term;
    }
    return sum;
}
