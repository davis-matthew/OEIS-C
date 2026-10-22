/*
 * A387611  a(n) = numerator(Integral_{(x,y,z) in R^3 : 0 <= x^2 + y^2 + z^2
 *          <= n^2} (x^2 + y^2) dx dy dz)/Pi.
 * https://oeis.org/A387611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 8*n^5/gcd(n,15) (numerator of 8*n^5/15)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    seq_t nn = (seq_t)n;

    /* Compute n^5 */
    seq_t num = seq_pow(nn, 5);

    /* Compute the gcd of n and 15 */
    seq_t g = gcd(nn, (seq_t)15);

    /* Divide by the gcd first to keep numbers small */
    seq_t div = num / g;

    /* Multiply by 8 to get the final result */
    seq_t result = div * (seq_t)8;

    return result;
}
