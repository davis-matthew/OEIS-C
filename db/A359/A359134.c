/*
 * A359134  a(n) = Sum_{d|n} (2*d)^(n/d - 1).
 * https://oeis.org/A359134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2*d)^(n/d - 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n ≥ 1, index starts at 1) */

#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d itself */
            idx_t exp1 = n / d - 1;
            seq_t base1 = (seq_t)(2 * d);
            sum += seq_pow(base1, exp1);

            /* paired divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t exp2 = n / d2 - 1;   /* which equals d - 1 */
                seq_t base2 = (seq_t)(2 * d2);
                sum += seq_pow(base2, exp2);
            }
        }
    }

    return sum;
}
