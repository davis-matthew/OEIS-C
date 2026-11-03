/*
 * A282031  Coefficients in q-expansion of (9*E_2(q^3) - E_2(q))/8.
 * https://oeis.org/A282031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n = 0 a(n) = 3*σ(n) - 27*σ(n/3) for n > 0 (σ: sum of
 *            divisors)
 * Range:     i8 n <= 17, i16 n <= 1889, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8515 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sigma_n = sigma_of(n);
    seq_t sigma_n3 = 0;
    if (n % 3 == 0)
        sigma_n3 = sigma_of(n / 3);
    return (seq_t)3 * sigma_n - (seq_t)27 * sigma_n3;
}
