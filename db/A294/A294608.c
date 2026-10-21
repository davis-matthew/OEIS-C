/*
 * A294608  a(n) = Sum_{d|n} d^(d + 1 + n/d).
 * https://oeis.org/A294608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d + 1 + n/d)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, undefined for 0 */
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            seq_t exp1 = d1 + 1 + d2;
            total += big_pow((seq_t)d1, exp1);
            if (d1 != d2) {
                seq_t exp2 = d2 + 1 + d1;
                total += big_pow((seq_t)d2, exp2);
            }
        }
    }
    return total;
}
