/*
 * A376015  a(n) = Sum_{d|n} d^n * binomial(n/d,d).
 * https://oeis.org/A376015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^n * binomial(n/d, d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            /* first divisor */
            sum += big_pow((seq_t)d1, (seq_t)n) * binomial(n / d1, d1);
            /* second divisor, distinct from first */
            if (d2 != d1) {
                sum += big_pow((seq_t)d2, (seq_t)n) * binomial(n / d2, d2);
            }
        }
    }
    return sum;
}
