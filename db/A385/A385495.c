/*
 * A385495  Most significant nonzero decimal digit of zeta(n)-1, where zeta(n)
 *          = Sum_{j >= 1} 1/j^n is the Riemann zeta function.
 * https://oeis.org/A385495
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = most significant nonzero decimal digit of ζ(n) - 1 ζ(n) =
 *            Σ_{j≥1} 1/jⁿ is the Riemann zeta function
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double LOG10_2 =
    0.3010299956639811952137388947244930267681898814621085418767L;

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;           /* defined only for n ≥ 2 */

    /* For n < 10 compute the sum directly.
       For n ≥ 10 the first non‑zero digit is given by the leading digit
       of 2⁻ⁿ and is obtained from fractional part of n·log10(2).      */
    if (n < 10)
    {
        long double s = 0.0L;
        const long double limit = 100000.0L;          /* sufficient for precision needed */
        for (long double j = 2.0L; j <= limit; ++j)
        {
            long double term = powl(1.0L / j, (long double)n);
            /* Tail error after j > limit is < 1/jⁿ ≤ (1/limit)ⁿ,
               which is far below the 10⁻³ precision needed for the digit. */
            s += term;
        }

        /* Find the first non‑zero decimal digit of s (0 < s < 1). */
        long double p = 1.0L;
        while (s * p < 1.0L)
            p *= 10.0L;
        int d = (int)floorl(s * p + 1e-12L);
        return (seq_t)d;
    }

    /* n ≥ 10: use leading digit of 2⁻ⁿ. */
    long double t   = (long double)n * LOG10_2;          /* t = n·log10(2) */
    long double f   = t - floorl(t);                    /* fractional part (0 ≤ f < 1) */
    long double val = powl(10.0L, 1.0L - f);             /* 10^(1-f) */
    int d = (int)floorl(val + 1e-12L);
    return (seq_t)d;
}
