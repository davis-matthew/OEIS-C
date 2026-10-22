/*
 * A365707  Initial digit of n^(n+1) (A007778(n)).
 * https://oeis.org/A365707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((n^(n+1))/10^floor(log10(n^(n+1))))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Leading digit of n^(n+1) */
    if (n == 0)                    /* 0^1 = 0, leading digit 0 */
        return (seq_t)0;

    /* Compute (n+1) * log10(n) using long double for extra precision */
    long double ln = (long double)(n + 1) * log10l((long double)n);
    long double frac = ln - floorl(ln);      /* fractional part */

    /* Compute 10^frac and take floor to get the first digit */
    /* Add a tiny epsilon to avoid cases where the result is just below an integer */
    long double d = powl(10.0L, frac);
    int digit = (int)(floorl(d + 1e-12L));

    return (seq_t)digit;
}
