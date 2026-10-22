/*
 * A321809  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^12.
 * https://oeis.org/A321809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d + n/d) * d^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 38, i128 n <= 1534,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but keep safe */
    seq_t result = 0;
    const idx_t exp = 12;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d = n / i;
            seq_t pow_i = pow_int((seq_t)i, exp);
            seq_t pow_d = (i == d) ? 0 : pow_int((seq_t)d, exp);
            unsigned parity = (i + d) & 1;
            seq_t term_i = parity ? -pow_i : pow_i;
            if (i == d) {
                result += term_i;
            } else {
                seq_t term_d = parity ? -pow_d : pow_d;
                result += term_i + term_d;
            }
        }
    }
    return result;
}
