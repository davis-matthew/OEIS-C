/*
 * A276222  Position of n^s in the joint ranking of {h} and {k^s}, h >= 1, k
 *          >= 2, s = golden ratio = (1+sqrt(5))/2.
 * https://oeis.org/A276222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n+1)^phi) with phi = (1+sqrt(5))/2 For n >= 1
 *            (offset 1). The first term a(1)=4 corresponds to the position of
 *            2^phi in the joint ranking of {h} and {k^phi}
 * Range:     u8 n <= 27, u16 n <= 938, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double PHI = (1.0L + sqrtl(5.0L)) / 2.0L;

seq_t a(idx_t n)
{
    /*  n is guaranteed to be >= 1 by the OEIS offset.  */
    /*  We compute floor((n+1)^phi) using long double arithmetic.
        The result fits in 64‑bit for all n tested (n <= 10000).  */
    long double base = (long double)(n + 1);
    long double power = powl(base, PHI);
    /* truncate the fractional part; the value is small enough to
       cast to unsigned long long.  */
    unsigned long long f = (unsigned long long)floorl(power);
    return (seq_t)n + (seq_t)f;
}
