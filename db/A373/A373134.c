/*
 * A373134  Expansion of e.g.f. (1/2)*(x^2*exp(x))*(sinh(x))^2.
 * https://oeis.org/A373134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   gcd, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(n,2)*(3^(n-2) + (-1)^n - 2)/4
 * Offset 0   (a(0) = 0)
 */
#include "../utils/pow3.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Values are 0 for n < 4 by definition of the sequence. */
    if (n < 4u) return 0;

    /* (3^(n-2) - 1) for even n, (3^(n-2) - 3) for odd n. */
    seq_t pow3_val = pow3(n - 2u);
    seq_t d = (n & 1u) ? (pow3_val - (seq_t)3u) : (pow3_val - (seq_t)1u);

    /* Reduce numerator by dividing by 8 using gcd */
    seq_t rem = (seq_t)8u;
    seq_t f0 = (seq_t)n;
    seq_t f1 = f0 - (seq_t)1u;
    seq_t f2 = d;

    /* Divide the consecutive factors by the required power of 2 */
    seq_t g = gcd(f0, rem);
    f0 /= g;
    rem /= g;
    g = gcd(f1, rem);
    f1 /= g;
    rem /= g;
    g = gcd(f2, rem);
    f2 /= g;
    rem /= g;
    /* rem should now be 1 */

    /* Multiply the reduced factors to get the result */
    seq_t tmp = f0 * f1;
    seq_t result = tmp * f2;
    return result;
}
