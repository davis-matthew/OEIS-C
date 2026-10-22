/*
 * A363645  Expansion of Sum_{k>0} x^k/(1 - k*x^k)^4.
 * https://oeis.org/A363645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(d-1) * binomial(d+2,3)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/_pow_seq_t.h"

/* Compute base^exp for seq_t using exponentiation by squaring. */

seq_t a(idx_t n) {
    /* n must be >= 1.  For n < 1 the behaviour is unspecified. */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t base = (seq_t)(n / d);
            seq_t pow = _pow_seq_t(base, d - 1);
            seq_t binom = ((seq_t)(d + 2) * (d + 1) * d) / 6;
            sum += pow * binom;

            /* paired divisor n/d if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                base = (seq_t)(n / d2);
                pow = _pow_seq_t(base, d2 - 1);
                binom = ((seq_t)(d2 + 2) * (d2 + 1) * d2) / 6;
                sum += pow * binom;
            }
        }
    }
    return sum;
}
