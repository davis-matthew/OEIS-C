/*
 * A290775  Number of 5-cycles in the n-triangular honeycomb bishop graph.
 * https://oeis.org/A290775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2/5 * binomial(n + 1, 4)*(8 - 7*n + 2*n^2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by cancelling the common divisor 120 = 5·24
   during the multiplicative construction of the binomial coefficient.   */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;

    /* Compute s = 2*n^2 + 8 - 7*n */
    seq_t s = (seq_t)2 * n0 * n0 + (seq_t)8;
    s -= (seq_t)7 * n0;

    /* Denominator 120 = 5*24, to be cancelled with the factors below. */
    seq_t denom = (seq_t)120;
    seq_t result = (seq_t)1;
    seq_t f;

    /* Factor n + 1 */
    f = n0 + (seq_t)1;
    seq_t g = gcd(f, denom);
    f /= g; denom /= g;
    result *= f;

    /* Factor n */
    f = n0;
    g = gcd(f, denom);
    f /= g; denom /= g;
    result *= f;

    /* Factor n - 1 */
    f = n0 - (seq_t)1;
    g = gcd(f, denom);
    f /= g; denom /= g;
    result *= f;

    /* Factor n - 2 */
    f = n0 - (seq_t)2;
    g = gcd(f, denom);
    f /= g; denom /= g;
    result *= f;

    /* Factor the constant 2 coming from the 2/5 prefactor */
    f = (seq_t)2;
    g = gcd(f, denom);
    f /= g; denom /= g;
    result *= f;

    /* Factor the remaining polynomial part s */
    f = s;
    g = gcd(f, denom);
    f /= g; denom /= g;
    result *= f;

    /* Finish any remaining division by the denominator. */
    if (denom != (seq_t)1)
        result /= denom;

    return result;
}
