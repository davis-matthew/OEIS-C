/*
 * A342677  a(n) = Sum_{d|n} (n/d)^(n-d+1).
 * https://oeis.org/A342677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(n-d+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t base1 = (seq_t)(n / d1);
            seq_t exp1  = (seq_t)(n - d1 + 1);
            sum += big_pow(base1, exp1);

            /* divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t base2 = (seq_t)(n / d2);
                seq_t exp2  = (seq_t)(n - d2 + 1);
                sum += big_pow(base2, exp2);
            }
        }
    }
    return sum;
}
