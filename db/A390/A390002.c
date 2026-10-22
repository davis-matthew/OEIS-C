/*
 * A390002  Leading digit of 15^n.
 * https://oeis.org/A390002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10^frac(n*log10(15)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    static const long double LOG15 = 1.17609125905568124212L;  /* log10(15) */
    long double x   = (long double)n * LOG15;          /* n·log10(15) */
    long double f   = x - floorl(x);                  /* fractional part */
    long double val = powl(10.0L, f);                  /* 10^frac */
    const long double EPS = 1e-12L;                   /* offset to avoid boundary */
    unsigned int digit = (unsigned int)(val + EPS);   /* leading decimal digit 1‑9 */

    /* Construct the result in seq_t from the small integer value. */
    seq_t result = digit;
    return result;
}
