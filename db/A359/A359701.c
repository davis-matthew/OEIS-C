/*
 * A359701  a(n) = Sum_{d|n} d^(d + n/d - 2).
 * https://oeis.org/A359701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} d^(d + n/d - 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for n ≥ 1 using divisor enumeration and big_pow. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1, value for n=0 is irrelevant       */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;                         /* other divisor */
            seq_t e1 = (seq_t)d + (seq_t)q - 2;
            sum += big_pow((seq_t)d, e1);
            if (d != q) {
                seq_t e2 = (seq_t)q + (seq_t)d - 2;
                sum += big_pow((seq_t)q, e2);
            }
        }
    }
    return sum;
}
