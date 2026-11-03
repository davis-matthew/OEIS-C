/*
 * A320901  Expansion of Sum_{k>=1} x^k/(1 + x^k)^4.
 * https://oeis.org/A320901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} (-1)^(d+1) * d*(d+1)*(d+2) / 6
 * Range:     i8 n <= 7, i16 n <= 55, i32 n <= 1810, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 8567 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline seq_t term_div(idx_t d)
{
    seq_t dd = (seq_t)d;                  /* promote divisor */
    seq_t t  = dd * (dd + 1) * (dd + 2) / 6; /* product of three consecutive integers */
    if ((d & 1) == 0)                     /* even d → negative sign */
        t = -t;
    return t;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Enumerate divisors in pairs (d, n/d) avoiding overflow in d*d */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += term_div(d);
            idx_t d2 = n / d;
            if (d2 != d)
                sum += term_div(d2);
        }
    }
    return sum;
}
