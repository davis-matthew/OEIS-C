/*
 * A342629  a(n) = Sum_{d|n} (n/d)^(n-d).
 * https://oeis.org/A342629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(n-d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            seq_t base1 = (seq_t)(n / d1);
            seq_t exp1  = (seq_t)(n - d1);
            total += big_pow(base1, exp1);

            idx_t d2 = n / d1;
            if (d2 != d1) {
                seq_t base2 = (seq_t)(n / d2);
                seq_t exp2  = (seq_t)(n - d2);
                total += big_pow(base2, exp2);
            }
        }
    }
    return total;
}
