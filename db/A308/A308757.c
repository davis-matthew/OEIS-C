/*
 * A308757  a(n) = Sum_{d|n} d^(3*(d-2)).
 * https://oeis.org/A308757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(3*(d-2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   int_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;                      /* sum over divisors */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            if (d == 1) {               /* 1^(negative) treated as 1 */
                sum += 1;
            } else {
                idx_t exp = 3 * (d - 2);
                sum += int_pow((seq_t)d, exp);
            }
        }
    }
    return sum;
}
