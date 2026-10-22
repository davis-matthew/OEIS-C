/*
 * A308696  a(n) = Sum_{d|n} d^(2*d).
 * https://oeis.org/A308696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(2*d)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;  // undefined for n<1, but safe guard

    /* iterate over all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* term for divisor d: d^(2*d) */
            seq_t term_d = big_pow((seq_t)d, (seq_t)(2 * d));
            sum += term_d;

            idx_t d2 = n / d;
            if (d2 != d) {
                /* term for complementary divisor d2: d2^(2*d2) */
                seq_t term_d2 = big_pow((seq_t)d2, (seq_t)(2 * d2));
                sum += term_d2;
            }
        }
    }
    return sum;
}
