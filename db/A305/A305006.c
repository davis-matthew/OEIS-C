/*
 * A305006  Numerators of coefficients in expansion of Sum_{k>=1} x^k/(k*(1 +
 *          x^k)).
 * https://oeis.org/A305006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of (∑_{d|n} (-1)^(d+1) * d) / n i.e. a(n) =
 *            S(n) / gcd(|S(n)|, n) where S(n) = ∑_{d|n} (-1)^(d+1) * d This
 *            computation is performed on the fly, requiring only O(√n) work
 *            per call
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"


seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;           /* undefined for offset 0, safe guard */

    seq_t S = (seq_t)0;

    /* Enumerate divisors by iterating up to sqrt(n). */
    for (idx_t d = (idx_t)1; d <= n / d; ++d) {
        if (n % d == (idx_t)0) {
            /* Sign for divisor d: +1 if d is odd, -1 if d is even. */
            seq_t sign_d = (d & (idx_t)1) ? (seq_t)1 : (seq_t)-1;
            S += sign_d * (seq_t)d;

            idx_t e = n / d;                     /* Complementary divisor */
            if (e != d) {
                seq_t sign_e = (e & (idx_t)1) ? (seq_t)1 : (seq_t)-1;
                S += sign_e * (seq_t)e;
            }
        }
    }

    /* Reduce the fraction S / n to lowest terms and return the numerator. */
    seq_t absS = S >= (seq_t)0 ? S : -S;
    seq_t g = gcd(absS, (seq_t)n);
    return S / g;
}
