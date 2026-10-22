/*
 * A359700  a(n) = Sum_{d|n} d^(d + n/d - 1).
 * https://oeis.org/A359700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} d^(d + n/d - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t co = n / d;

            seq_t d_val = (seq_t)d;
            seq_t exp_d = (seq_t)(d + co - 1);
            sum += big_pow(d_val, exp_d);

            if (co != d) {
                seq_t co_val = (seq_t)co;
                seq_t exp_co = (seq_t)(co + d - 1);
                sum += big_pow(co_val, exp_co);
            }
        }
    }
    return sum;
}
