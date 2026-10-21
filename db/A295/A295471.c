/*
 * A295471  a(n) = (20*n)!*n!/((10*n)!*(8*n)!*(3*n)!).
 * https://oeis.org/A295471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(20*n,10*n)*binom(10*n,2*n)/binom(3*n,n)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute the three binomial factors */
    idx_t twon   = 20 * n;
    idx_t ten_n  = 10 * n;
    idx_t two_n  = 2 * n;
    idx_t three_n = 3 * n;

    seq_t t1    = binom(twon,  ten_n);   /* binom(20n,10n)   */
    seq_t t2    = binom(ten_n, two_n);   /* binom(10n,2n)    */
    seq_t denom = binom(three_n, n);     /* binom(3n, n)     */

    /* cancel common factors to keep intermediate values small */
    seq_t g = gcd(t1, denom);
    t1 /= g;
    denom /= g;

    g = gcd(t2, denom);
    t2 /= g;
    denom /= g;          /* denom should now be 1 */

    /* if, for any unlikely reason, denom != 1, fall back to full division */
    if (denom != (seq_t)1) {
        seq_t num = t1;
        num *= t2;
        return num / denom;
    }
    return t1 * t2;
}
