/*
 * A321561  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^5.
 * https://oeis.org/A321561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^5
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 73, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow5(seq_t x)
{
    seq_t r = x;
    r *= x;  // x^2
    r *= x;  // x^3
    r *= x;  // x^4
    r *= x;  // x^5
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        idx_t e = n / d;

        /* divisor d */
        seq_t term = pow5((seq_t)d);
        if ( ((d + e) & 1ULL) == 0 )
            result += term;
        else
            result -= term;

        /* symmetric divisor e, skip if same */
        if (e != d)
        {
            term = pow5((seq_t)e);
            if ( ((e + d) & 1ULL) == 0 )
                result += term;
            else
                result -= term;
        }
    }
    return result;
}
