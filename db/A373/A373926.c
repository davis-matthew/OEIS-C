/*
 * A373926  a(n) = Sum_{d|n} (-1)^phi(d) * mu(n/d).
 * https://oeis.org/A373926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^phi(d) * mu(n/d)
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-01)
 * Helpers:   mobius, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but keep safe */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* divisor d */
        seq_t sign = ((totient(d) & 1) ? -1 : 1);
        seq_t mu = mobius(n / d);   /* mu(n/d) */
        sum += sign * mu;

        /* paired divisor n/d, avoid double counting for perfect squares */
        idx_t e = n / d;
        if (e != d) {
            seq_t sign_e = ((totient(e) & 1) ? -1 : 1);
            seq_t mu_e = mobius(d);   /* mu(n/e) = mu(d) */
            sum += sign_e * mu_e;
        }
    }
    return sum;
}
