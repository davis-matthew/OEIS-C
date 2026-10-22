/*
 * A358660  a(n) = Sum_{d|n} d * (n/d)^(n-d).
 * https://oeis.org/A358660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * (n/d)^(n-d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* compute for divisor d1 */
            seq_t base1 = (seq_t)(n / d1);
            seq_t exp1  = (seq_t)(n - d1);
            seq_t term1 = (seq_t)d1 * big_pow(base1, exp1);
            sum += term1;

            if (d2 != d1) {
                /* compute for divisor d2 */
                seq_t base2 = (seq_t)(n / d2);
                seq_t exp2  = (seq_t)(n - d2);
                seq_t term2 = (seq_t)d2 * big_pow(base2, exp2);
                sum += term2;
            }
        }
    }
    return sum;
}
