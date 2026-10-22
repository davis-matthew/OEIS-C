/*
 * A376018  a(n) = Sum_{d|n} d^d * binomial(n/d-1,d-1).
 * https://oeis.org/A376018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * binomial(n/d-1,d-1)
 * Range:     u8 n <= 15, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t term = big_pow((seq_t)d, (seq_t)d);
            term *= binomial(n / d - 1, d - 1);
            result += term;
            /* divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                term = big_pow((seq_t)d2, (seq_t)d2);
                term *= binomial(n / d2 - 1, d2 - 1);
                result += term;
            }
        }
    }
    return result;
}
