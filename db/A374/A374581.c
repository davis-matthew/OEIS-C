/*
 * A374581  a(n) is the denominator of (120*n^2 + 151*n + 47)/(512*n^4 +
 *          1024*n^3 + 712*n^2 + 194*n + 15).
 * https://oeis.org/A374581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (120*n^2 + 151*n + 47)/(512*n^4 + 1024*n^3
 *            + 712*n^2 + 194*n + 15)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 53, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;          /* n^2 */
    seq_t sn3 = sn2 * sn;         /* n^3 */
    seq_t sn4 = sn2 * sn2;        /* n^4 */

    seq_t num = 120 * sn2 + 151 * sn + 47;          /* numerator */
    seq_t denom = 512 * sn4 + 1024 * sn3 + 712 * sn2 + 194 * sn + 15; /* denominator */

    seq_t g = gcd(num, denom);    /* greatest common divisor */
    return denom / g;             /* reduced denominator */
}
