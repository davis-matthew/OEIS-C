/*
 * A344061  a(n) = Sum_{d|n} sigma(d)^(n/d).
 * https://oeis.org/A344061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma(d)^(n/d)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d1;

            seq_t base1 = sigma_of(d1);
            seq_t exp1  = (seq_t)(n / d1);
            sum += big_pow(base1, exp1);

            if (d1 != d2) {
                seq_t base2 = sigma_of(d2);
                seq_t exp2  = (seq_t)(n / d2);
                sum += big_pow(base2, exp2);
            }
        }
    }
    return sum;
}
