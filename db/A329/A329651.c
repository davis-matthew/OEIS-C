/*
 * A329651  Expansion of x * (psi(x^6) / psi(-x^3))^3 * phi(-x)^5 / psi(-x) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A329651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = s(n) - 12*s(n/2) + 27*s(n/3) - 16*s(n/4) where s(x) is
 *            the sum of divisors of x and s(x)=0 if x is not an integer
 *            (i.e., if n is not divisible by the denominator)
 * Range:     i8 n <= 14, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t result = sigma_of(n);
    if (n % 2 == 0) result -= 12 * sigma_of(n / 2);
    if (n % 3 == 0) result += 27 * sigma_of(n / 3);
    if (n % 4 == 0) result -= 16 * sigma_of(n / 4);
    return result;
}
