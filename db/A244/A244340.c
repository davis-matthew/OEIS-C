/*
 * A244340  Floor( n^2*log(n) ) mod n.
 * https://oeis.org/A244340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2*log(n)) mod n
 * Range:     u8 n <= 10, u16 n <= 233, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety, sequence is defined for n>=1 */
    double dn = (double)n;
    double v = dn * dn * log(dn);
    seq_t val = (seq_t)((long long)v);
    return val % n;
}
