/*
 * A359189  a(n) = Sum_{d|n} d * 3^(n/d-1).
 * https://oeis.org/A359189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * 3^(n/d - 1) (Offset = 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

/* Computes the OEIS sequence A359189 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            idx_t exp = n / d - 1;
            seq_t pow = pow_uint(3, exp);     /* 3^(n/d-1) */
            seq_t term = pow * (seq_t)d;      /* d * 3^(n/d-1) */
            sum += term;
        }
    }
    return sum;
}
