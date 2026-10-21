/*
 * A241807  Numerators of c(n) = (n^2+n+2)/((n+1)*(n+2)*(n+3)) as defined in
 *          A241269.
 * https://oeis.org/A241807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of reduced fraction (n^2 + n +
 *            2)/((n+1)(n+2)(n+3))
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;                     /* convert index */
    seq_t num = nt * nt + nt + 2;            /* n^2 + n + 2 */
    seq_t den = (nt + 1) * (nt + 2) * (nt + 3); /* (n+1)(n+2)(n+3) */
    seq_t g   = gcd(num, den);               /* gcd of numerator and denominator */
    return num / g;                          /* reduced numerator */
}
