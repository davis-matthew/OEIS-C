/*
 * A264854  a(n) = n*(n + 1)*(11*n^2 + 11*n - 10)/24.
 * https://oeis.org/A264854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(11*n^2 + 11*n - 10)/24
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t t1 = nn;                    /* n */
    seq_t t2 = nn + 1;                /* n+1 */
    seq_t t3 = ((seq_t)11 * nn * nn) + ((seq_t)11 * nn) - (seq_t)10; /* 11 n^2 + 11 n - 10 */
    seq_t denom = 24;
    seq_t g;

    g = gcd(t1, denom);
    t1 /= g;
    denom /= g;

    g = gcd(t2, denom);
    t2 /= g;
    denom /= g;

    g = gcd(t3, denom);
    t3 /= g;
    denom /= g;                       /* denom should now be 1 */

    seq_t result = t1 * t2;
    result *= t3;
    return result;
}
