/*
 * A337970  L.g.f.: -log( Sum_{n=-oo..+oo} (-p)^n * (p*x)^(n^2) ) = Sum_{n>=1}
 *          a(n) * x^n/n, where p = sqrt(5).
 * https://oeis.org/A337970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} (n/d) * ( if ((n/d) even) 5^(d*(n/d)/2) else (5^d
 *            + 1) * 5^(d*((n/d)-1)/2) ) where the sum runs over all positive
 *            divisors d of n. This follows from the logarithmic generating
 *            function in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* process divisor d */
            idx_t t = n / d;
            seq_t mult = (seq_t)(t);
            if (t % 2 == 0) {
                idx_t exp = d * (t / 2);
                seq_t term = power5(exp);
                res += term * mult;
            } else {
                idx_t exp = d * ((t - 1) / 2);
                seq_t term = power5(exp);
                seq_t powdn = power5(d);
                seq_t factor = powdn + 1;
                term = term * factor;
                res += term * mult;
            }

            /* process complementary divisor e = n/d if distinct */
            idx_t e = n / d;
            if (e != d) {
                t = n / e;               /* equals d */
                mult = (seq_t)(t);
                if (t % 2 == 0) {
                    idx_t exp = e * (t / 2);
                    seq_t term = power5(exp);
                    res += term * mult;
                } else {
                    idx_t exp = e * ((t - 1) / 2);
                    seq_t term = power5(exp);
                    seq_t powde = power5(e);
                    seq_t factor = powde + 1;
                    term = term * factor;
                    res += term * mult;
                }
            }
        }
    }
    return res;
}
