/*
 * A388283  Numerator of (3*n - sigma(n)) / n.
 * https://oeis.org/A388283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = (3*n - sigma(n)) / gcd( n, 3*n - sigma(n) )
 * Range:     i8 n <= 66, i16 n <= 9769, i32 n <= 9769, i64 n <= 9769,
 *            i128 n <= 9769, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset undefined, harmless */
    seq_t sn   = (seq_t)n;
    seq_t sig  = sigma_of(n);
    seq_t num  = (seq_t)3 * sn - sig;
    seq_t g    = gcd(num, sn);
    return num / g;
}
