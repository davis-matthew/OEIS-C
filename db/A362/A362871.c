/*
 * A362871  Leading digit of 6^n.
 * https://oeis.org/A362871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = leading digit of 6^n, computed as floor(10^{ frac(
 *            n·log10(6) ) }) where frac(x)=x-floor(x)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    static const long double log10_6 = 0.7781512503836436L;
    long double f = (long double)n * log10_6;
    long double frac = f - floorl(f);
    long double ld = powl(10.0L, frac);
    /* Tiny epsilon to avoid rounding down when ld is extremely close to an integer. */
    int d = (int)(ld + 1e-12L);
    return (seq_t)d;
}
