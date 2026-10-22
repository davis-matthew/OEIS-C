/*
 * A363093  Leading digit of 7^n.
 * https://oeis.org/A363093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = leading decimal digit of 7^n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Leading digit of 7^n can be obtained from the fractional part
       of n * log10(7).  For n up to 10000 double precision is more
       than sufficient. */
    static const double log10_7 = 0.8450980400142568;  /* log10(7) */

    /* n=0 satisfies 7^0=1 */
    if (n == 0)
        return (seq_t)1;

    double t = (double)n * log10_7;
    double frac = t - floor(t);          /* fractional part of t */
    double first = pow(10.0, frac);      /* 10^frac, between 1 and 10 */

    /* Add a tiny epsilon before casting to handle rounding errors */
    int digit = (int)(first + 1e-12);
    return (seq_t)digit;
}
