/*
 * A364789  Initial digit of (n^n)^n (A002489(n)).
 * https://oeis.org/A364789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(((n^n)^n)/10^floor(log10((n^n)^n)))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading digit of n^(n^2) */
#include <math.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by convention (0^0 = 1) */
    if (n == 0)
        return (seq_t)1;

    /* Use long double to obtain the fractional part of n^2 * log10(n) */
    long double nd   = (long double)n;
    long double prod = nd * nd * log10l(nd);      /* n^2 * log10(n) */
    long double frac = prod - floorl(prod);       /* fractional part */

    /* 10^frac gives the most significant digits in [1,10) */
    long double mant = powl(10.0L, frac);

    /* Convert to an integer 1…9.  Add a tiny epsilon to avoid
       rounding down when mant is extremely close to an integer. */
    uint64_t digit = (uint64_t)(mant + 1e-12L);

    return (seq_t)digit;
}
